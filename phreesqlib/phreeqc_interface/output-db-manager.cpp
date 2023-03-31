/********************************************************************************
*  This file is part of PhreeSQL                                                *
*  Copyright(C) 2023: Daniela Cabiddu                                           *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Daniela Cabiddu (daniela.cabiddu@cnr.it)                                  *
*                                                                               *
*     Italian National Research Council (CNR)                                   *
*     Institute for Applied Mathematics and Information Technologies (IMATI)    *
*     Via de Marini, 6                                                          *
*     16149 Genoa,                                                              *
*     Italy                                                                     *
*                                                                               *
*  This program is free software: you can redistribute it and/or modify it      *
*  under the terms of the GNU General Public License as published by the        *
*  Free Software Foundation, either version 3 of the License, or (at your       *
*  option) any later version.                                                   *
*                                                                               *
*  This program is distributed in the hope that it will be useful, but          *
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY   *
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
*  more details.                                                                *
*                                                                               *
*  You should have received a copy of the GNU General Public License along      *
*  with this program. If not, see <https://www.gnu.org/licenses/>.              *
*                                                                               *
*********************************************************************************/

#include "output-db-manager.h"

inline
bool OutputDBManager::isOutputEmpty()
{
    int num_tables;
    query = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='" + metadata_table_name + "';";
    rc = sqlite3_exec(db, query.c_str(), emptyDBCallback, &num_tables, &err_message);
    this->queryResult(rc, "check if empty");
    return num_tables == 0;
}

inline
bool OutputDBManager::createEpsgTable(const std::string table_name)
{
    query = "CREATE TABLE IF NOT EXISTS " + table_name + " "
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "ANALYSIS_ID INTEGER NOT NULL,"
            "COORD_X DOUBLE,"
            "COORD_Y DOUBLE,"
            "COORD_Z DOUBLE,"
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating epsg table");

    return (rc == SQLITE_OK);
}

inline
void OutputDBManager::createAnalisysTable()
{
    query = "CREATE TABLE IF NOT EXISTS " + metadata_table_name +
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "JOB_TYPE TEXT,"
            "SURVEY TEXT,"
            "SITE_NAME TEXT,"
            "DATE DATETIME,"
            "DATABASE TEXT,"
            "PHREEQC_VERSION TEXT,"
            "RUN_NUMBER INTEGER,"
            "SAMPLE_NAME TEXT,"
            "INPUT_FILE TEXT,"
            "COORD_X DOUBLE,"
            "COORD_Y DOUBLE,"
            "COORD_Z DOUBLE,"
            "EPSG INT,"
            "TIMESTAMP DATETIME,"
            "TITLE TEXT,"
            "SOLUTION TEXT,"
            "UNIQUE(DATABASE, INPUT_FILE) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating analisys table");
}

inline
void OutputDBManager::createSolutionCompositionTable()
{
    query = "CREATE TABLE IF NOT EXISTS SOLUTION_COMPOSITION "
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "ANALYSIS_ID INTEGER NOT NULL,"
            "ELEMENTS TEXT,"
            "MOLALITY DOUBLE,"
            "MOLES DOUBLE,"
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating solution_composition");
}

inline
void OutputDBManager::createDescriptionOfSolutionTable()
{
    query = "CREATE TABLE IF NOT EXISTS DESCRIPTION_OF_SOLUTION "
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "ANALYSIS_ID INTEGER NOT NULL,"
            "PARAM TEXT,"
            "VALUE DOUBLE, "
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating description_of_solution");
}

inline
void OutputDBManager::createDistributionOfSpeciesTable()
{
    query = "CREATE TABLE IF NOT EXISTS DISTRIBUTION_OF_SPECIES "
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "ANALYSIS_ID INTEGER NOT NULL,"
            "SPECIES TEXT,"
            "MOLALITY DOUBLE,"
            "ACTIVITY DOUBLE,"
            "LOG_MOLALITY DECIMAL (3, 3),"
            "LOG_ACTIVITY DECIMAL (3, 3),"
            "LOG_GAMMA DECIMAl (3, 3),"
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating distribution_of_species");
}

inline
void OutputDBManager::createDistributionOfAlkalinityTable()
{
    query = "CREATE TABLE IF NOT EXISTS DISTRIBUTION_OF_ALKALINITY "
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "ANALYSIS_ID INTEGER NOT NULL,"
            "SPECIES TEXT,"
            "ALKALINITY DOUBLE,"
            "MOLALITY DOUBLE,"
            "ALK_MOL DECIMAL (2, 2),"
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating distribution_of_alkalinity");
}

inline
void OutputDBManager::createSaturationIndicesTable()
{
    query = "CREATE TABLE IF NOT EXISTS SATURATION_INDICES "
            "( ID INTEGER PRIMARY KEY NOT NULL, "
            "ANALYSIS_ID INTEGER NOT NULL,"
            "PHASE TEXT,"
            "SI DECIMAL (3 ,2),"
            "LOG_IAP DECIMAL (3 ,2),"
            "LOG_K DECIMAL (3 ,2),"
            "FORMULA TEXT,"
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "creating saturation_indices");
}

inline
bool OutputDBManager::insertEpsg(const std::string table_name,
               const std::vector<std::string> &id,
               const std::vector<double> &x,
               const std::vector<double> &y,
               const std::vector<double> &z)
{
    for (uint i=0; i < id.size(); i++)
    {
        query = "INSERT OR REPLACE INTO " + table_name + " (ANALYSIS_ID, COORD_X, COORD_Y, COORD_Z) VALUES ('" +
                id.at(i).c_str() + "', '" +
                to_string(x.at(i)) + "', '" +
                to_string(y.at(i)) + "', '" +
                to_string(z.at(i)) + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

        if (rc == SQLITE_OK)
        {
//                this->queryResult(rc, "inserting epsg");
        }
        else
        {
            std::cerr << "\033[1;31mSQL ERROR " << rc << ": " << err_message << "\033[0m" << std::endl;
        }
    }

    return (rc == SQLITE_OK);
}

inline
int OutputDBManager::insertAnalisys(Analisys &a, metadata meta)
{
    query = "INSERT OR REPLACE INTO " + metadata_table_name + " (JOB_TYPE, SURVEY, SITE_NAME, DATE, DATABASE, PHREEQC_VERSION, RUN_NUMBER, SAMPLE_NAME, INPUT_FILE, COORD_X, COORD_Y, COORD_Z, EPSG, TIMESTAMP, TITLE, SOLUTION) VALUES ('" +
            a.job_type + "', '" +
            a.survey + "', '" +
            a.site_name + "', '" +
            a.date + "', '" +
            a.database + "', '" +
            a.phreeqc_version + "', '" +
            a.run_number + "', '" +
            a.sample_name + "', '" +
            a.input_file + "', '" +
            a.coord_x + "', '" +
            a.coord_y + "', '" +
            a.coord_z + "', '" +
            a.epsg + "', '" +
            a.timestamp + "', '" +
            meta["TITLE"] + "', '" +
            meta["SOLUTION"] + "');";

    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    if (rc == SQLITE_OK)
    {
        this->queryResult(rc, "inserting analisys");
        a.id = sqlite3_last_insert_rowid(db);
        this->a_id = a.id;
    }
    else
    {
        std::cerr << "\033[1;31mSQL ERROR " << rc << ": " << err_message << "\033[0m" << std::endl;

        switch (rc)
        {
        case SQLITE_CONSTRAINT:
            std::cerr << "\033[1;31mAnalysis " << a.input_file << " may already be in the database computed with the same PHREEQC database and it is considered as a duplicate IGNORED.\033[0m" << std::endl;
        break;
        default:
            std::cerr << "" << std::endl;
        }
    }

    return rc;
}

inline
void OutputDBManager::insertSolutionComposition(vector<SolutionComposition> sc_list)
{
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    for (int i = 0; i < sc_list.size(); i++)
    {
        query = "INSERT OR REPLACE INTO SOLUTION_COMPOSITION (ANALYSIS_ID, ELEMENTS, MOLALITY, MOLES) VALUES ('" +
                to_string(a_id) + "', '" +
                sc_list[i].element + "', '" +
                sc_list[i].molality + "', '" +
                sc_list[i].moles + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
        this->queryResult(rc, "inserting solution_composition");
    }

    sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
}

inline
void OutputDBManager::insertDescriptionOfSolution(vector<DescriptionOfSolution> des_list)
{
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    for (int i = 0; i < des_list.size(); i++)
    {
        query = "INSERT OR REPLACE INTO DESCRIPTION_OF_SOLUTION (ANALYSIS_ID, PARAM, VALUE) VALUES ('" +
                to_string(a_id) + "', '" +
                des_list[i].key_name + "', '" +
                des_list[i].value + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
        this->queryResult(rc, "inserting description_of_solution");
    }

    sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
}

inline
void OutputDBManager::insertDistributionOfSpecies(vector<DistributionOfSpecies> dis_list)
{
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    for (int i = 0; i < dis_list.size(); i++)
    {
        query = "INSERT OR REPLACE INTO DISTRIBUTION_OF_SPECIES (ANALYSIS_ID, SPECIES, MOLALITY, ACTIVITY, LOG_MOLALITY, LOG_ACTIVITY, LOG_GAMMA) VALUES ('" +
                to_string(a_id) + "', '" +
                dis_list[i].species + "', '" +
                dis_list[i].molality + "', '" +
                dis_list[i].activity + "', '" +
                dis_list[i].log_molality + "', '" +
                dis_list[i].log_activity + "', '" +
                dis_list[i].log_gamma + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
        this->queryResult(rc, "inserting distribution_of_species");
    }

    sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
}

inline
void OutputDBManager::insertDistributionOfAlkalinity(vector<DistributionOfAlkalinity> alk_list)
{
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    for (int i = 0; i < alk_list.size(); i++)
    {
        query = "INSERT OR REPLACE INTO DISTRIBUTION_OF_ALKALINITY (ANALYSIS_ID, SPECIES, ALKALINITY, MOLALITY, ALK_MOL) VALUES ('" +
                to_string(a_id) + "', '" +
                alk_list[i].species + "', '" +
                alk_list[i].alkalinity + "', '" +
                alk_list[i].molality + "', '" +
                alk_list[i].alk_mol + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
        this->queryResult(rc, "inserting distribution_if_alkalinity");
    }

    sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
}

inline
void OutputDBManager::insertSaturationIndices(vector<SaturationIndices> si_list)
{
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    for (int i = 0; i < si_list.size(); i++)
    {
        query = "INSERT OR REPLACE INTO SATURATION_INDICES (ANALYSIS_ID, PHASE, SI, LOG_IAP, LOG_K, FORMULA) VALUES ('" +
                to_string(a_id) + "', '" +
                si_list[i].phase + "', '" +
                si_list[i].si + "', '" +
                si_list[i].log_iap + "', '" +
                si_list[i].log_k + "', '" +
                si_list[i].formula + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
        this->queryResult(rc, "insert saturation_indicies");
    }

    sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
}

inline
bool OutputDBManager::selectValuesfromAnalisys(Analisys &a, int analysis_id)
{
    char **res;
    int row, column;
    SolutionComposition sc;

    query = "SELECT * FROM " + metadata_table_name + " WHERE ID='" + to_string(analysis_id) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selecting analisys");

    if (row == 0 || column == 0)
    {
        std::cerr << __FUNCTION__ << " ERROR : Unexisting analysis ID " << analysis_id << std::endl;
        return false;
    }

    a.id = atoi(res[0 + column]);
    a.job_type = res[1 + column];
    a.survey = res[2 + column];
    a.site_name = res[3 + column];
    a.date = res[4 + column];
    a.database = res[5 + column];
    a.phreeqc_version = res[6 + column];
    a.run_number = res[7 + column];
    a.sample_name = res[8 + column];
    a.input_file = res[9 + column];
    a.coord_x = res[10 + column];
    a.coord_y = res[11 + column];
    a.coord_z = res[12 + column];
    a.epsg = res[13 + column];
    a.timestamp = res[14 + column];

    sqlite3_free_table(res);

    return true;
}

inline
void OutputDBManager::selectValuesFromSolutionComposition(vector<SolutionComposition> &sc_list, int analisys_num)
{
    char **res;
    int row, column;
    SolutionComposition sc;

    query = "SELECT * FROM SOLUTION_COMPOSITION WHERE ANALYSIS_ID='" + to_string(analisys_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selection solution_composition");

    sc_list.clear();

    for (int i = 5; i < (row + 1) * column; i = i + 5)
    {
        sc.analisys_id = atoi(res[i + 1]);
        sc.element = res[i + 2];
        sc.molality = res[i + 3];
        sc.moles = res[i + 4];
        sc_list.push_back(sc);
    }
    sqlite3_free_table(res);
}

inline
void OutputDBManager::selectValuesFromDescriptionOfSolution(vector<DescriptionOfSolution> &des_list, int analisys_num)
{
    char **res;
    int row, column;
    DescriptionOfSolution des;

    query = "SELECT * FROM DESCRIPTION_OF_SOLUTION WHERE ANALYSIS_ID='" + to_string(analisys_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selection distribution_of_solution");

    des_list.clear();

    for (int i = 4; i < (row + 1) * column; i = i + 4)
    {
        des.analisys_id = atoi(res[i + 1]);
        des.key_name = res[i + 2];
        des.value = res[i + 3];
        des_list.push_back(des);
    }
    //printDescriptionOfSolution(des_list);
    sqlite3_free_table(res);
}

inline
void OutputDBManager::selectValuesFromDistributionOfSpecies(vector<DistributionOfSpecies> &dis_list, int analisys_num)
{
    char **res;
    int row, column;
    DistributionOfSpecies dis;

    query = "SELECT * FROM DISTRIBUTION_OF_SPECIES WHERE ANALYSIS_ID='" + to_string(analisys_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selectiong distribution_of_species");

    dis_list.clear();

    for (int i = 8; i < (row + 1) * column; i = i + 8)
    {
        dis.analisys_id = atoi(res[i + 1]);
        dis.species = res[i + 2];
        dis.molality = res[i + 3];
        dis.activity = res[i + 4];
        dis.log_molality = res[i + 5];
        dis.log_activity = res[i + 6];
        dis.log_gamma = res[i + 7];
        dis_list.push_back(dis);
    }
    //printDistributionOfSpecies(dis_list);
    sqlite3_free_table(res);
}

inline
void OutputDBManager::selectValuesFromDistributionOfAlkalinity(vector<DistributionOfAlkalinity> &alk_list, int analisys_num)
{
    char **res;
    int row, column;
    DistributionOfAlkalinity alk;

    query = "SELECT * FROM DISTRIBUTION_OF_ALKALINITY WHERE ANALYSIS_ID='" + to_string(analisys_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selection distribution_of_alkalinity");

    alk_list.clear();

    for (int i = 6; i < (row + 1) * column; i = i + 6)
    {
        alk.analisys_id = atoi(res[i + 1]);
        alk.species = res[i + 2];
        alk.alkalinity = res[i + 3];
        alk.molality = res[i + 4];
        alk.alk_mol = res[i + 5];
        alk_list.push_back(alk);
    }
    //printDistributionOfAlkalinity(alk_list);
    sqlite3_free_table(res);
}

inline
void OutputDBManager::selectValuesFromSaturationIndices(vector<SaturationIndices> &si_list, int analisys_num)
{
    char **res;
    int row, column;
    SaturationIndices si;

    query = "SELECT * FROM SATURATION_INDICES WHERE ANALYSIS_ID='" + to_string(analisys_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selection of saturation_indicies");

    si_list.clear();

    for (int i = 7; i < (row + 1) * column; i = i + 7)
    {
        si.analisys_id = atoi(res[i + 1]);
        si.phase = res[i + 2];
        si.si = res[i + 3];
        si.log_iap = res[i + 4];
        si.log_k = res[i + 5];
        si.formula = res[i + 6];
        si_list.push_back(si);
    }
    //printSaturationIndices(si_list);
    sqlite3_free_table(res);
}