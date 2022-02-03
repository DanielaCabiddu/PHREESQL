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

class MatracReader
{
private:
    InputDBManager *i_manager;
    OutputDBManager *o_manager;
    DataManager *d_manager;
    InputReaderWriter i_file;
    OutputReaderWriter o_file;
    string db_path;
    sqlite3 *db;
    int rc;
    bool is_i_file;
    bool is_o_file;

    bool openDB()
    {
        rc = sqlite3_open(db_path.c_str(), &db);

        if (rc)
        {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return false;
        }
        else
        {
            sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
//            fprintf(stderr, "");
            return true;
        }
    }

    void closeDB()
    {
        sqlite3_close(db);
    }

    void deleteFileIfExists(string path)
    {
        ofstream file;
        file.open(path);
        if (file)
        {
            remove(path.c_str());
        }
    }

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
    MatracReader(string db_path)
    {
        this->db_path = db_path;
        openDB();

        i_manager = new InputDBManager(db);
        o_manager = new OutputDBManager(db);
        d_manager = new DataManager(db);
    }

    ~MatracReader()
    {
        delete i_manager;
        delete o_manager;
        delete d_manager;

        sqlite3_close(db);
    }

    void readInputOutputFiles(string input_path, string output_path, string meta_path)
    {
//        readOutputFile(output_path, meta_path);
//        readInputFile(input_path);

        readIOMFiles(input_path, output_path, meta_path);
    }

    bool readIOMFiles(string ifile_path, string ofile_path, string meta_path)
    {
        if (i_file.readFile(ifile_path) && o_file.readFile(ofile_path) && o_file.readMetadata(meta_path))
        {
            this->createInputTables();
            this->createOutputTables();
            o_manager->createAnalisysTable();
            o_manager->insertAnalisys(o_file.a, i_file.meta);

            i_manager->insertSolutionInputs(i_file.input_list, (o_file.a).id);

            o_manager->insertSolutionComposition(o_file.sc_list);
            o_manager->insertDescriptionOfSolution(o_file.des_list);
            o_manager->insertDistributionOfSpecies(o_file.dis_list);
            o_manager->insertDistributionOfAlkalinity(o_file.alk_list);
            o_manager->insertSaturationIndices(o_file.si_list);

            std::cout << std::endl << " ============== REPORT ================== " << std::endl << std::endl;

            std::cout << o_manager->metadata_table_name << " #rows : " << o_manager->getNumRows(o_manager->metadata_table_name) << std::endl;
            std::cout << "SOLUTION_INPUT" << " #rows : " << o_manager->getNumRows("SOLUTION_INPUT") << std::endl;

            return true;
        }
        else
        {
            cerr << "Error in opening either input/output or metadata file " << endl;
            return false;
        }

    }

    void writeInputFile(string directory, int analysis_id)
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
                i_file.writeFile(analysis_id, directory);
//            }
            closedir(dir);
        }
        else
            return;
    }

    void writeInputFiles(string directory, std::vector<int> analysis_ids)
    {
        for (const int analysis_id : analysis_ids)
            writeInputFile(directory, analysis_id);
    }

    void writeAllInputFiles(string directory)
    {
        int num = o_manager->getNumRows(o_manager->metadata_table_name);
        for (int analysis_id = 1; analysis_id <= num; analysis_id++)
        {
            writeInputFile(directory, analysis_id);
        }
    }


    void writeOutputFile(string directory, int analysis_id)
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
            this->deleteFileIfExists(file_path);

            o_file.writeAnalisysFile(file_path, analysis_id, i_file.input_list);
//        }

    }

    void writeOutputFiles(string directory, std::vector<int> analysis_ids)
    {
        for (const int analysis_id : analysis_ids)
            writeOutputFile(directory, analysis_id);
    }

    void writeAllOutputFiles(string directory)
    {
        int num = o_manager->getNumRows(o_manager->metadata_table_name);
        for (int analysis_id = 1; analysis_id <= num; analysis_id++)
        {
            writeOutputFile(directory, analysis_id);
        }
    }

    void createInputTables()
    {
//        i_manager->createMetadataTable();
        i_manager->createSolutionInputTable();
    }

    void createOutputTables()
    {
        o_manager->createSolutionCompositionTable();
        o_manager->createDescriptionOfSolutionTable();
        o_manager->createDistributionOfSpeciesTable();
        o_manager->createDistributionOfAlkalinityTable();
        o_manager->createSaturationIndicesTable();
    }

    vector<vector<string> > getData(string table_name, string value, string column_name, string timestamp)
    {
        return d_manager->getData(table_name, value, column_name, timestamp);
    }

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
};
