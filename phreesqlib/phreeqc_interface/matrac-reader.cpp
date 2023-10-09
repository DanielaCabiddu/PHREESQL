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

#include "matrac-reader.h"

inline
PhreeqcInterface::PhreeqcInterface(const string db_path)
{
    this->db_path = db_path;
    bool success = openDB();

    if (!success)
    {
        std::cerr << "error opening " << db_path << std::endl;
        return;
    }

    i_manager = new InputDBManager(db);
    o_manager = new OutputDBManager(db);
    d_manager = new DataManager(db);

    bool empty = d_manager->isDBEmpty();
}

inline
bool PhreeqcInterface::openDB()
{
    rc = sqlite3_open(db_path.c_str(), &db);

    if (rc != 0)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    else
    {
//            std::cout << "DB " << db_path << " open." << std::endl;
        int rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
//            fprintf(stderr, "");
        return true;
    }
}

inline
void PhreeqcInterface::closeDB()
{
    sqlite3_close(db);
}

inline
void PhreeqcInterface::deleteFileIfExists(string path)
{
    ofstream file;
    file.open(path);
    if (file)
    {
        remove(path.c_str());
    }
}
inline
DIR * PhreeqcInterface::openDir(string directory)
{
    DIR *dir = opendir(directory.c_str());
    if (!dir)
    {
        int success;

#ifdef WIN32
        success = _mkdir(directory.c_str());
#else
        success = mkdir(directory.c_str(), 0777/*, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH*/);
#endif


        if (success == -1)
        {
            cerr << "Error in creating directory!" << endl;
            return NULL;
        }
        else
        {
            dir = opendir(directory.c_str());
        }
    }
    return dir;
}

inline
void PhreeqcInterface::copyDB (const std::string file_path)
{
    std::ifstream src(db_path, std::ios::binary);
    std::ofstream dest(file_path, std::ios::binary);
    dest << src.rdbuf();
}

inline
std::vector<std::vector<std::pair<std::string, std::string>>> PhreeqcInterface::getMetadata () const
{
    std::vector<std::vector<std::pair<std::string, std::string>>> res = d_manager->getMetadata();
    return res;
}

inline
void PhreeqcInterface::readInputOutputFiles(string input_path, string output_path, string meta_path)
{
    readIOMFiles(input_path, output_path, meta_path);
}

inline
bool PhreeqcInterface::readIOMFiles(string ifile_path, string ofile_path, string meta_path)
{
    if (i_file.readFile(ifile_path) && o_file.readFile(ofile_path) && o_file.readMetadata(meta_path))
    {
        this->createInputTables();
        this->createOutputTables();
        o_manager->createAnalisysTable();

        int result = o_manager->insertAnalisys(o_file.a, i_file.meta);

        if (result == SQLITE_OK)
        {
            i_manager->insertSolutionInputs(i_file.input_list, (o_file.a).id);

            o_manager->insertSolutionComposition(o_file.sc_list);
            o_manager->insertDescriptionOfSolution(o_file.des_list);
            o_manager->insertDistributionOfSpecies(o_file.dis_list);
            o_manager->insertDistributionOfAlkalinity(o_file.alk_list);
            o_manager->insertSaturationIndices(o_file.si_list);
        }

        return true;
    }
    else
    {
        cerr << "Error in opening either input/output or metadata file " << endl;
        return false;
    }

}
inline
void PhreeqcInterface::writeInputFile(string directory, int analysis_id, const bool overwrite)
{
    std::cout << "Writing Input File - Analysis ID : [" << analysis_id << "]" << std::endl;

    if (i_manager->isDBEmpty())
    {
        cerr << "Error: Empty Database" << endl;
        return;
    }

    if (i_manager->isInputEmpty())
    {
        cerr << "Error: No input tables found" << endl;
        return;
    }

//        if (i_file == NULL)
//        {
//            i_file = new InputReaderWriter();
//        }

    DIR *dir = this->openDir(directory);

    if (dir != NULL)
    {
//            int num = o_manager->getNumRows(o_manager->metadata_table_name);
//            for (int i = 1; i <= num; i++)
//            {
            i_manager->selectValuesFromMetadata(i_file.meta, analysis_id);
            i_manager->selectValuesFromSolution(i_file.input_list, analysis_id);
            this->deleteFileIfExists(directory + "/dump_input_" + to_string(analysis_id) + ".txt");

            const std::string filename = i_file.meta["INPUT_FILE"].substr(0, i_file.meta["INPUT_FILE"].find_last_of(("."))) + ".pqi";
            const std::string full_path = directory + separator() + filename;

            if (!overwrite && fileExists(full_path))
            {
                std::cerr << "Input file " << full_path << " already exists. Set the --overwrite flag to overwrite." << std::endl;
                return;
            }

            i_file.writeFile(analysis_id, full_path);
//            }
        closedir(dir);
    }
    else
        return;
}

inline
void PhreeqcInterface::writeInputFiles(string directory, std::vector<int> analysis_ids, const bool overwrite)
{
    for (const int analysis_id : analysis_ids)
        writeInputFile(directory, analysis_id, overwrite);
}

inline
void PhreeqcInterface::writeAllInputFiles(string directory, const bool overwrite)
{
    int num = o_manager->getNumRows(o_manager->metadata_table_name);
    for (int analysis_id = 1; analysis_id <= num; analysis_id++)
    {
        writeInputFile(directory, analysis_id, overwrite);
    }
}

    //////////////////////////////////////////////////////////////////////////////////////

inline
void PhreeqcInterface::writeMetadataFile(string directory, int analysis_id, const bool overwrite)
{
    std::cout << "Writing Metadata File - Analysis ID : [" << analysis_id << "]" << std::endl;

    if (i_manager->isDBEmpty())
    {
        cerr << "Error: Empty Database" << endl;
        return;
    }

    if (i_manager->isInputEmpty())
    {
        cerr << "Error: No input tables found" << endl;
        return;
    }

//        if (i_file == NULL)
//        {
//            i_file = new InputReaderWriter();
//        }

    DIR *dir = this->openDir(directory);

    if (dir != NULL)
    {
//            int num = o_manager->getNumRows(o_manager->metadata_table_name);
//            for (int i = 1; i <= num; i++)
//            {
            o_manager->selectValuesfromAnalisys(o_file.a, analysis_id);
//                i_manager->selectValuesFromMetadata(i_file.meta, analysis_id);

            const std::string filename = o_file.a.input_file.substr(0, o_file.a.input_file.find_last_of(("."))) + ".met";

            const std::string file_path = directory + separator() + filename ;

            if (!overwrite && fileExists(file_path))
            {
                std::cerr << "Metadata file " << file_path << " already exists. Set the --overwrite flag to overwrite." << std::endl;
                return;
            }

            o_file.writeMetadata(file_path.c_str());
//            }
        closedir(dir);
    }
    else
        return;
}

inline
void PhreeqcInterface::writeMetadataFiles(string directory, std::vector<int> analysis_ids, const bool overwrite)
{
    for (const int analysis_id : analysis_ids)
        writeMetadataFile(directory, analysis_id, overwrite);
}

inline
void PhreeqcInterface::writeAllMetadataFiles(string directory, const bool overwrite)
{
    int num = o_manager->getNumRows(o_manager->metadata_table_name);
    for (int analysis_id = 1; analysis_id <= num; analysis_id++)
    {
        writeMetadataFile(directory, analysis_id, overwrite);
    }
}

    //////////////////////////////////////////////////////////////////////////////////////

inline
void PhreeqcInterface::writeOutputFile(string directory, int analysis_id, const bool overwrite)
{
    if (o_manager->isDBEmpty())
    {
        cerr << "Error: Empty Database" << endl;
        return;
    }

    if (o_manager->isOutputEmpty())
    {
        cerr << "Error: No output tables found" << endl;
        return;
    }

//        if (o_file == NULL)
//        {
//            o_file = new OutputReaderWriter();
//        }

//        int num = o_manager->getNumRows(o_manager->metadata_table_name);

//        for (int i = 1; i <= num; i++)
//        {
        std::cout << "Writing Output File - Analysis ID : [" << analysis_id << "]" << std::endl;

        bool exists = o_manager->selectValuesfromAnalisys(o_file.a, analysis_id);

        if (!exists)
        {
            std::cerr << "error - exporting analysis id " << analysis_id << std::endl;
            return;
        }

        i_manager->selectValuesFromSolution(i_file.input_list, analysis_id);

        o_manager->selectValuesFromSolutionComposition(o_file.sc_list, analysis_id);
        o_manager->selectValuesFromDescriptionOfSolution(o_file.des_list, analysis_id);
        o_manager->selectValuesFromDistributionOfSpecies(o_file.dis_list, analysis_id);
        o_manager->selectValuesFromDistributionOfAlkalinity(o_file.alk_list, analysis_id);
        o_manager->selectValuesFromSaturationIndices(o_file.si_list, analysis_id);

        const std::string filename = o_file.a.input_file.substr(0, o_file.a.input_file.find_last_of(("."))) + ".pqo";
        const std::string file_path = directory + separator() + filename ;

        if (!overwrite && fileExists(file_path))
        {
            std::cerr << "Output file " << file_path << " already exists. Set the --overwrite flag to overwrite." << std::endl;
            return;
        }


        o_file.writeAnalisysFile(file_path, analysis_id, i_file.input_list);
//        }

}

inline
void PhreeqcInterface::writeOutputFiles(string directory, std::vector<int> analysis_ids, const bool overwrite)
{
    for (const int analysis_id : analysis_ids)
        writeOutputFile(directory, analysis_id, overwrite);
}

inline
void PhreeqcInterface::writeAllOutputFiles(string directory, const bool overwrite)
{
    int num = o_manager->getNumRows(o_manager->metadata_table_name);
    for (int analysis_id = 1; analysis_id <= num; analysis_id++)
    {
        writeOutputFile(directory, analysis_id, overwrite);
    }
}

inline
void PhreeqcInterface::createInputTables()
{
//        i_manager->createMetadataTable();
    i_manager->createSolutionInputTable();
}

inline
void PhreeqcInterface::createOutputTables()
{
    o_manager->createSolutionCompositionTable();
    o_manager->createDescriptionOfSolutionTable();
    o_manager->createDistributionOfSpeciesTable();
    o_manager->createDistributionOfAlkalinityTable();
    o_manager->createSaturationIndicesTable();
}

inline
bool PhreeqcInterface::create_and_insert_EpsgTable(const std::string table_name,
                                 const std::vector<std::string> &id,
                                 const std::vector<double> &x,
                                 const std::vector<double> &y,
                                 const std::vector<double> &z)
{
//        const std::string table_name = "EPSG_" + std::to_string(epsg);
    bool success = o_manager->createEpsgTable(table_name);

    if (success)
        success = o_manager->insertEpsg(table_name, id, x, y, z);

    return success;
}

inline
bool PhreeqcInterface::updateAnalysisEPSG (const unsigned int epsg,
                         const std::vector<std::string> &id,
                         const std::vector<double> &x,
                         const std::vector<double> &y)
{
    return d_manager->updateMetadataEPSG(epsg, id, x, y);
}

inline
vector<vector<string> > PhreeqcInterface::getData(string table_name, string value, string column_name, string timestamp)
{
    return d_manager->getData(table_name, value, column_name, timestamp);
}

inline
void PhreeqcInterface::writeData(string file_path, vector<vector<string> > data)
{
    if (!data.empty())
    {
        d_manager->writeToFile(file_path, data);
    }
    else
    {
        cerr << "Error: no data" << endl;
    }
}

inline
std::string PhreeqcInterface::getMetadataTableName () const
{
    return o_manager->metadata_table_name;
}

inline
unsigned int PhreeqcInterface::getNumMetadataRecords () const
{
    return o_manager->getNumRows(o_manager->metadata_table_name);
}

