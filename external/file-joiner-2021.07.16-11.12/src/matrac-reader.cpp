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
            if (mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
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

            o_manager->insertSolutionComposition(o_file.sc_list);
            o_manager->insertDescriptionOfSolution(o_file.des_list);
            o_manager->insertDistributionOfSpecies(o_file.dis_list);
            o_manager->insertDistributionOfAlkalinity(o_file.alk_list);
            o_manager->insertSaturationIndices(o_file.si_list);

            return true;
        }
        else
        {
            cerr << "Error in opening either input/output or metadata file " << endl;
            return false;
        }

    }

    void writeInputFile(string directory)
    {
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
            int num = o_manager->getNumRows("METADATA");
            for (int i = 1; i <= num; i++)
            {
                i_manager->selectValuesFromMetadata(i_file.meta, i);
                i_manager->selectValuesFromSolution(i_file.input_list, i);
                this->deleteFileIfExists(directory + "/dump_input_" + to_string(i) + ".txt");
                i_file.writeFile(i, directory);
            }
            closedir(dir);
        }
        else
            return;
    }

    void writeOutputFile(string file_path)
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

        this->deleteFileIfExists(file_path);

        int num = o_manager->getNumRows("ANALISYS");

        for (int i = 1; i <= num; i++)
        {
            o_manager->selectValuesfromAnalisys(o_file.a, i);
            o_manager->selectValuesFromSolutionComposition(o_file.sc_list, i);
            o_manager->selectValuesFromDescriptionOfSolution(o_file.des_list, i);
            o_manager->selectValuesFromDistributionOfSpecies(o_file.dis_list, i);
            o_manager->selectValuesFromDistributionOfAlkalinity(o_file.alk_list, i);
            o_manager->selectValuesFromSaturationIndices(o_file.si_list, i);
            o_file.writeAnalisysFile(file_path, i);
        }
        this->closeDB();
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
