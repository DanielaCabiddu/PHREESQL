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

#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "utils.h"
#include "models.h"
#include "input-db-manager.cpp"
#include "output-db-manager.cpp"
#include "data-manager.cpp"
#include "input-reader-writer.cpp"
#include "output-reader-writer.cpp"

using namespace std;

///
/// \brief The MatracReader class
///
class MatracReader
{
private:
    ///
    /// \brief i_manager
    ///
    InputDBManager *i_manager;

    ///
    /// \brief o_manager
    ///
    OutputDBManager *o_manager;

    ///
    /// \brief d_manager
    ///
    DataManager *d_manager;

    ///
    /// \brief i_file
    ///
    InputReaderWriter i_file;

    ///
    /// \brief o_file
    ///
    OutputReaderWriter o_file;

    ///
    /// \brief db_path
    ///
    string db_path;

    ///
    /// \brief db
    ///
    sqlite3 *db;

    ///
    /// \brief rc
    ///
    int rc;

    ///
    /// \brief is_i_file
    ///
    bool is_i_file;

    ///
    /// \brief is_o_file
    ///
    bool is_o_file;

    ///
    /// \brief openDB
    /// \return
    ///
    bool openDB()
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

    ///
    /// \brief closeDB
    ///
    void closeDB()
    {
        sqlite3_close(db);
    }

    ///
    /// \brief deleteFileIfExists
    /// \param path
    ///
    void deleteFileIfExists(string path)
    {
        ofstream file;
        file.open(path);
        if (file)
        {
            remove(path.c_str());
        }
    }

    ///
    /// \brief openDir
    /// \param directory
    /// \return
    ///
    DIR *openDir(string directory)
    {
        DIR *dir = opendir(directory.c_str());
        if (!dir)
        {
            if (mkdir(directory.c_str(), 0777/*, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH*/) == -1)
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

public:
    ///
    /// \brief MatracReader
    /// \param db_path
    ///
    MatracReader(string db_path)
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

    ~MatracReader()
    {
        delete i_manager;
        delete o_manager;
        delete d_manager;

        sqlite3_close(db);
    }

    ///
    /// \brief copyDB
    /// \param file_path
    ///
    void copyDB (const std::string file_path)
    {
        std::ifstream src(db_path, std::ios::binary);
        std::ofstream dest(file_path, std::ios::binary);
        dest << src.rdbuf();
    }

    ///
    /// \brief getMetadata
    /// \return
    ///
    std::vector<std::vector<std::pair<std::string, std::string>>> getMetadata ()
    {
        std::vector<std::vector<std::pair<std::string, std::string>>> res = d_manager->getMetadata();
        return res;
    }

    ///
    /// \brief readInputOutputFiles
    /// \param input_path
    /// \param output_path
    /// \param meta_path
    ///
    void readInputOutputFiles(string input_path, string output_path, string meta_path)
    {
//        readOutputFile(output_path, meta_path);
//        readInputFile(input_path);

        readIOMFiles(input_path, output_path, meta_path);
    }

    ///
    /// \brief readIOMFiles
    /// \param ifile_path
    /// \param ofile_path
    /// \param meta_path
    /// \return
    ///
    bool readIOMFiles(string ifile_path, string ofile_path, string meta_path)
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

    ///
    /// \brief writeInputFile
    /// \param directory
    /// \param analysis_id
    /// \param overwrite
    ///
    void writeInputFile(string directory, int analysis_id, const bool overwrite)
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

    ///
    /// \brief writeInputFiles
    /// \param directory
    /// \param analysis_ids
    /// \param overwrite
    ///
    void writeInputFiles(string directory, std::vector<int> analysis_ids, const bool overwrite)
    {
        for (const int analysis_id : analysis_ids)
            writeInputFile(directory, analysis_id, overwrite);
    }

    ///
    /// \brief writeAllInputFiles
    /// \param directory
    /// \param overwrite
    ///
    void writeAllInputFiles(string directory, const bool overwrite)
    {
        int num = o_manager->getNumRows(o_manager->metadata_table_name);
        for (int analysis_id = 1; analysis_id <= num; analysis_id++)
        {
            writeInputFile(directory, analysis_id, overwrite);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////

    ///
    /// \brief writeMetadataFile
    /// \param directory
    /// \param analysis_id
    /// \param overwrite
    ///
    void writeMetadataFile(string directory, int analysis_id, const bool overwrite)
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

    ///
    /// \brief writeMetadataFiles
    /// \param directory
    /// \param analysis_ids
    /// \param overwrite
    ///
    void writeMetadataFiles(string directory, std::vector<int> analysis_ids, const bool overwrite)
    {
        for (const int analysis_id : analysis_ids)
            writeMetadataFile(directory, analysis_id, overwrite);
    }

    ///
    /// \brief writeAllMetadataFiles
    /// \param directory
    /// \param overwrite
    ///
    void writeAllMetadataFiles(string directory, const bool overwrite)
    {
        int num = o_manager->getNumRows(o_manager->metadata_table_name);
        for (int analysis_id = 1; analysis_id <= num; analysis_id++)
        {
            writeMetadataFile(directory, analysis_id, overwrite);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////

    ///
    /// \brief writeOutputFile
    /// \param directory
    /// \param analysis_id
    /// \param overwrite
    ///
    void writeOutputFile(string directory, int analysis_id, const bool overwrite)
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

    ///
    /// \brief writeOutputFiles
    /// \param directory
    /// \param analysis_ids
    /// \param overwrite
    ///
    void writeOutputFiles(string directory, std::vector<int> analysis_ids, const bool overwrite)
    {
        for (const int analysis_id : analysis_ids)
            writeOutputFile(directory, analysis_id, overwrite);
    }

    ///
    /// \brief writeAllOutputFiles
    /// \param directory
    /// \param overwrite
    ///
    void writeAllOutputFiles(string directory, const bool overwrite)
    {
        int num = o_manager->getNumRows(o_manager->metadata_table_name);
        for (int analysis_id = 1; analysis_id <= num; analysis_id++)
        {
            writeOutputFile(directory, analysis_id, overwrite);
        }
    }

    ///
    /// \brief createInputTables
    ///
    void createInputTables()
    {
//        i_manager->createMetadataTable();
        i_manager->createSolutionInputTable();
    }

    ///
    /// \brief createOutputTables
    ///
    void createOutputTables()
    {
        o_manager->createSolutionCompositionTable();
        o_manager->createDescriptionOfSolutionTable();
        o_manager->createDistributionOfSpeciesTable();
        o_manager->createDistributionOfAlkalinityTable();
        o_manager->createSaturationIndicesTable();
    }

    ///
    /// \brief create_and_insert_EpsgTable
    /// \param table_name
    /// \param id
    /// \param x
    /// \param y
    /// \param z
    /// \return
    ///
    bool create_and_insert_EpsgTable(const std::string table_name,
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

    ///
    /// \brief updateAnalysisEPSG
    /// \param epsg
    /// \param id
    /// \param x
    /// \param y
    /// \return
    ///
    bool updateAnalysisEPSG (const uint epsg,
                             const std::vector<std::string> &id,
                             const std::vector<double> &x,
                             const std::vector<double> &y)
    {
        return d_manager->updateMetadataEPSG(epsg, id, x, y);
    }

    ///
    /// \brief getData
    /// \param table_name
    /// \param value
    /// \param column_name
    /// \param timestamp
    /// \return
    ///
    vector<vector<string> > getData(string table_name, string value, string column_name, string timestamp)
    {
        return d_manager->getData(table_name, value, column_name, timestamp);
    }

    ///
    /// \brief writeData
    /// \param file_path
    /// \param data
    ///
    void writeData(string file_path, vector<vector<string> > data)
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

    ///
    /// \brief getMetadataTableName
    /// \return
    ///
    std::string getMetadataTableName () const
    {
        return o_manager->metadata_table_name;
    }

    ///
    /// \brief getNumMetadataRecords
    /// \return
    ///
    uint getNumMetadataRecords () const
    {
        return o_manager->getNumRows(o_manager->metadata_table_name);
    }
};
