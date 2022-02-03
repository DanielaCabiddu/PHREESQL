#include "PhreeSQLibEngine.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

int main(int argc, char *argv[])
{
    std::string in_folder;
    std::string out_folder;
    std::string meta_folder;
    std::string db;

    std::string export_folder;
    std::string export_ids_list_filename;
    int export_analysis_id = INT_MAX;

    std::string phreeqc_db_path;

    int export_input = 0;
    int export_output = 0;
    int export_metadata = 0;
    int run_phreeqc = 0;
    int fill_db = 0;

    while (1)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
//            {"export_input",    no_argument,       &export_input,  0},
            {"export_input",    no_argument,      &export_input,    1},
            {"export_output",   no_argument,      &export_output,   1},
            {"export_metadata", no_argument,      &export_metadata, 1},
            {"fill_db",         no_argument,      &fill_db,         1},
            {"run_phreeqc",     no_argument,      &run_phreeqc,     1},
            /* These options don’t set a flag.
             We distinguish them by their indices. */
            //      {"add",     no_argument,       0, 'a'},
            {"export_folder",  required_argument, 0, 'F'},
            {"export_id",  required_argument, 0, 'I'},
            {"export_list_ids",  required_argument, 0, 'L'},

            {"phreeqc_db",  required_argument, 0, 'P'},

            {"database",      required_argument, 0, 'd'},
            {"in_folder",     required_argument, 0, 'i'},
            {"out_folder",    required_argument, 0, 'o'},
            {"meta_folder",   required_argument, 0, 'm'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        int c = getopt_long (argc, argv, "d:F:i:I:o:m:P:S:", long_options, &option_index);

        if (c==-1) break;

        switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            printf ("option %s set to true\n", long_options[option_index].name);
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 'd':
          printf ("option -d (database) with value `%s'\n", optarg);
          db = optarg;
          break;

        case 'F':
          printf ("option -F (export folder) with value `%s'\n", optarg);
          export_folder = optarg;
          break;

        case 'i':
          printf ("option -i (input folder) with value `%s'\n", optarg);
          in_folder = optarg;
          break;

        case 'I':
          printf ("option -I (export single analysis) with value `%s'\n", optarg);
          export_analysis_id = atoi(optarg);
          break;

        case 'L':
          printf ("option -I (export list of analysis) with value `%s'\n", optarg);
          export_ids_list_filename = optarg;
          break;

        case 'o':
          printf ("option -o (output folder) with value `%s'\n", optarg);
          out_folder = optarg;
          break;

        case 'm':
          printf ("option -m (meta folder) with value `%s'\n", optarg);
          meta_folder = optarg;
          break;

        case 'P':
          printf ("option -P (Phreeqc DB) with value `%s'\n", optarg);
          phreeqc_db_path = optarg;
          break;

        default:
          abort ();
        }
    }

    if (db.length() == 0)
    {
        std::cerr << "error - database parameter missing" << std::endl;
        return 1;
    }

    if (fill_db > 0)
    {
        if (in_folder.length() == 0 || out_folder.length() == 0 || meta_folder.length() == 0)
        {
            std::cerr << "error - in, out and meta folders must be provided as an input." << std::endl;
            return 1;
        }
    }

    if ((export_input > 0 || export_output > 0 || export_metadata > 0) && export_folder.length() == 0)
    {
        std::cerr << "error - export folder must be provided to perform export operations." << std::endl;
        return 1;
    }

    if (run_phreeqc > 0 && phreeqc_db_path.length()==0)
    {
        std::cerr << "error - PhreeQC DB must be provided to run PhreeQC. " << std::endl;
    }

    phreesqlib::PhreeSQLibEngine engine (db);

    if (run_phreeqc > 0)
    {
        std::cout << std::endl;
        std::cout << "========================================================================================" << std::endl;
        std::cout << "Running PhreeQC using DB " << phreeqc_db_path << "..." << std::endl;
        engine.run_phreeqc_on_folder(in_folder, out_folder, phreeqc_db_path);
    }

    if (fill_db > 0)
    {
        std::cout << std::endl;
        std::cout << "========================================================================================" << std::endl;
        std::cout << "Reading folders and filling database " << db << "..." << std::endl;

        engine.run_on_folder(in_folder, out_folder, meta_folder);
    }

    if (export_input > 0 || export_output > 0 || export_metadata)
    {
        std::vector<int> ids;

        if (export_ids_list_filename.length() > 0)
        {
            ////leggi file...
            std::ifstream list_file;
            list_file.open(export_ids_list_filename);

            if (!list_file.is_open())
            {
                std::cerr << "error - opening ID list file " << export_ids_list_filename << std::endl;
            }
            else
            {
                int id;
                while (list_file >> id)
                    ids.push_back(id);

                list_file.close();
            }
        }

        if (export_analysis_id < INT_MAX)
        {
            ids.push_back(export_analysis_id);
        }

        if (export_input > 0)
        {
            std::cout << "========================================================================================" << std::endl;
            std::cout << "Exporting inputs from " << db << "..." << std::endl;
            engine.export_input(export_folder, ids);
        }

        if (export_output > 0)
        {
            std::cout << "========================================================================================" << std::endl;
            std::cout << "Exporting outputs from " << db << "..." << std::endl;
            engine.export_output(export_folder, ids);
        }

        if (export_metadata > 0)
        {
            std::cout << "========================================================================================" << std::endl;
            std::cout << "Exporting metadata from " << db << "..." << std::endl;
            engine.export_metadata(export_folder, ids);
        }


    }

    return 0;
}
