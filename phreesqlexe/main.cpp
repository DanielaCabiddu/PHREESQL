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

#include "PhreeSQLibEngine.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// #include <getopt.h>

#include <tclap/CmdLine.h>

/*! \file
 *  \brief Main entry point for PhreeSQLExe application.
 *
 */

///
/// \brief Main entry point for PhreeSQLExe application.
///
/// Input options for PhreeSQLExe application are described in the table below:
/// \image html command_line_args.png width=800cm height=400cm
///
/// \return 0 if success, 1 otherwise
///
///
int main(int argc, char *argv[])
{
    std::string in_folder;
    std::string out_folder;
    std::string meta_folder;
    std::string db;
    int epsg = INT_MAX;

    std::string export_folder;
    std::string export_ids_list_filename;
    int export_analysis_id = INT_MAX;

    std::string phreeqc_db_path;

    int epsg_convert = 0;
    int export_input = 0;
    int export_output = 0;
    int export_metadata = 0;
    int export_all = 0;
    int run_phreeqc = 0;
    int fill_db = 0;
    int overwrite = 0;

    std::vector<phreesqlib::DBEngine::EPSG_CONVERT_TYPE> epsg_convert_types;
    std::vector<std::string> epsg_convert_outputs;

    // Wrap everything in a try block.  Do this every time,
    // because exceptions will be thrown for problems.
    try {

        // Define the command line object, and insert a message
        // that describes the program. The "Command description message"
        // is printed last in the help text. The second argument is the
        // delimiter (usually space) and the last one is the version number.
        // The CmdLine object parses the argv array based on the Arg objects
        // that it contains.
        TCLAP::CmdLine cmd("", ' ', "0.9");

        // Define a value argument and add it to the command line.
        // A value arg defines a flag and a type of value that it expects,
        // such as "-n Bishop".


        TCLAP::ValueArg<std::string> export_folder_arg    ("F","export_folder","Path of the output folder.",false,"","string");
        TCLAP::ValueArg<std::string> export_id_arg        ("I","export_id","The ID of the single analyses to be exported.",false,"","integer");
        TCLAP::ValueArg<std::string> export_list_ids_arg  ("L","export_list_ids","Path of a text file listing the IDS of the analyses to be exported.",false,"","string");

        TCLAP::ValueArg<std::string> phreeqc_db_arg ("P","phreeqc_db","Path of PHREEQC database to be used for the calculation.",false,"","string");

        TCLAP::ValueArg<std::string> database_arg   ("d","database","Reference to the database to be created/updated.",false,"","string");
        TCLAP::ValueArg<std::string> epsg_arg       ("e","epsg","Output EPSG.",false,"","string");

        TCLAP::ValueArg<std::string> out_filename_arg   ("f","out_filename","Path of the output file (.csv).",false,"","string");
        TCLAP::ValueArg<std::string> out_table_arg      ("t","out_table","Name of the output table to be created in the input PHREESQL database.",false,"","string");
        TCLAP::ValueArg<std::string> out_database_arg   ("b","out_database","Name of the output PHREESQL database having converted EPSG.",false,"","string");
        TCLAP::ValueArg<std::string> in_folder_arg      ("i","in_folder","Path of the folder containing PHREEQC inputs (.pqi).",false,"","string");
        TCLAP::ValueArg<std::string> out_folder_arg     ("o","out_folder","Path of the folder where PHREEQC output (.pqo) will be saved.",false,"","string");
        TCLAP::ValueArg<std::string> meta_folder_arg    ("m","meta_folder","Path of the folder containing metadata files (.met).",false,"","string");


        TCLAP::SwitchArg export_all_switch  ("","export_all","Export PHREEQC input, output and metadata from the database.", cmd, 1);
        TCLAP::SwitchArg export_in_switch   ("","export_input","Export PHREEQC input (.pqi) from the database.", cmd, 1);
        TCLAP::SwitchArg export_out_switch  ("","export_output","Export PHREEQC output (.pqo) from the database.", cmd, 1);
        TCLAP::SwitchArg export_meta_switch ("","export_metadata","Export PHREEQC metadata (.met) from the database.", cmd, 1);

        TCLAP::SwitchArg fill_db_switch     ("","fill_db","Create a new database or update an existing one.", cmd, 1);
        TCLAP::SwitchArg overwrite_switch   ("O","overwrite","If an input analysis is already in the database, overwrite it.", cmd, 1);
        TCLAP::SwitchArg phreeqc_switch     ("","run_phreeqc","Run speciation calculation.", cmd, 1);
        TCLAP::SwitchArg epsg_convert_switch("","epsg_convert","Export the database by converting the coordinates into a desired EPSG.", cmd, 1);

        cmd.add(export_folder_arg);
        cmd.add(export_id_arg);
        cmd.add(export_list_ids_arg);
        cmd.add(phreeqc_db_arg);
        cmd.add(database_arg);
        cmd.add(epsg_arg);
        cmd.add(out_filename_arg);
        cmd.add(out_table_arg);
        cmd.add(out_database_arg);
        cmd.add(in_folder_arg);
        cmd.add(out_folder_arg);
        cmd.add(meta_folder_arg);


        // Parse the argv array.
        cmd.parse( argc, argv );

        if (export_all_switch.isSet())
            export_all = 1;

        if (export_in_switch.isSet())
            export_input = 1;

        if (export_out_switch.isSet())
            export_output = 1;

        if (export_meta_switch.isSet())
            export_metadata = 1;

        if (fill_db_switch.isSet())
            fill_db = 1;

        if (overwrite_switch.isSet())
            overwrite = 1;

        if (phreeqc_switch.isSet())
            run_phreeqc = 1;

        if (epsg_convert_switch.isSet())
            epsg_convert = 1;

        if (export_folder_arg.isSet())
            export_folder = export_folder_arg.getValue();

        if (export_id_arg.isSet())
            export_analysis_id = std::atoi(export_id_arg.getValue().c_str());

        if (export_list_ids_arg.isSet())
            export_ids_list_filename = export_list_ids_arg.getValue();

        if (phreeqc_db_arg.isSet())
            phreeqc_db_path = phreeqc_db_arg.getValue();

        if (database_arg.isSet())
            db = database_arg.getValue();

        if (epsg_arg.isSet())
            epsg = std::atoi(epsg_arg.getValue().c_str());

        if (out_filename_arg.isSet())
        {
            epsg_convert_types.push_back(phreesqlib::DBEngine::EPSG_CONVERT_TYPE::CSV);
            epsg_convert_outputs.push_back(out_filename_arg.getValue());
        }

        if (out_table_arg.isSet())
        {
            epsg_convert_types.push_back(phreesqlib::DBEngine::EPSG_CONVERT_TYPE::TABLE);
            epsg_convert_outputs.push_back(out_table_arg.getValue());
        }

        if (out_database_arg.isSet())
        {
            epsg_convert_types.push_back(phreesqlib::DBEngine::EPSG_CONVERT_TYPE::DB);
            epsg_convert_outputs.push_back(out_database_arg.getValue());
        }

        if (in_folder_arg.isSet())
            in_folder = in_folder_arg.getValue();

        if (out_folder_arg.isSet())
            out_folder = out_folder_arg.getValue();

        if (meta_folder_arg.isSet())
            meta_folder = meta_folder_arg.getValue();


    }
        catch (exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }


    std::cout << "========================================================================================" << std::endl;
    std::cout << std::endl << argv[0] << " started with the following parameters: " << std::endl << std::endl;

//     while (1)
//     {;
//         static struct option long_options[] =
//         {
//             /* These options set a flag. */
// //            {"export_input",    no_argument,       &export_input,  0},
//             {"export_all",      no_argument,      &export_all,      1},
//             {"export_input",    no_argument,      &export_input,    1},
//             {"export_output",   no_argument,      &export_output,   1},
//             {"export_metadata", no_argument,      &export_metadata, 1},
//             {"e_a",             no_argument,      &export_all,      1},
//             {"e_i",             no_argument,      &export_input,    1},
//             {"e_o",             no_argument,      &export_output,   1},
//             {"e_m",             no_argument,      &export_metadata, 1},
//             {"fill_db",         no_argument,      &fill_db,         1},
//             {"overwrite",       no_argument,      &overwrite,       1},
//             {"run_phreeqc",     no_argument,      &run_phreeqc,     1},
//             {"epsg_convert",    no_argument,      &epsg_convert,    1},
//             /* These options don’t set a flag.
//              We distinguish them by their indices. */
//             //      {"add",     no_argument,       0, 'a'},
//             {"export_folder",  required_argument, 0, 'F'},
//             {"export_id",  required_argument, 0, 'I'},
//             {"export_list_ids",  required_argument, 0, 'L'},

//             {"phreeqc_db",  required_argument, 0, 'P'},

//             {"database",      required_argument, 0, 'd'},
//             {"epsg",          required_argument, 0, 'e'},
//             {"out_filename",  required_argument, 0, 'f'},
//             {"out_table",     required_argument, 0, 't'},
//             {"out_database",  required_argument, 0, 'b'},
//             {"in_folder",     required_argument, 0, 'i'},
//             {"out_folder",    required_argument, 0, 'o'},
//             {"meta_folder",   required_argument, 0, 'm'},
//             {0, 0, 0, 0}
//         };

//         /* getopt_long stores the option index here. */
//         int option_index = 0;

//         int c = getopt_long (argc, argv, "b:d:e:f:F:i:I:o:m:P:S:t:", long_options, &option_index);

//         if (c==-1) break;

//         switch (c)
//         {
//         case 0:
//           /* If this option set a flag, do nothing else now. */
//           if (long_options[option_index].flag != 0)
//             printf ("option %s set to true\n", long_options[option_index].name);
//             break;
//           printf ("option %s", long_options[option_index].name);
//           if (optarg)
//             printf (" with arg %s", optarg);
//           printf ("\n");
//           break;

//         case 'b':
//           printf ("option -b (export database) with value `%s'\n", optarg);
//           epsg_convert_types.push_back(phreesqlib::DBEngine::EPSG_CONVERT_TYPE::DB);
//           epsg_convert_outputs.push_back(optarg);
//           break;

//         case 'd':
//           printf ("option -d (database) with value `%s'\n", optarg);
//           db = optarg;
//           break;

//         case 'e':
//           printf ("option -e (epsg) with value `%s'\n", optarg);
//           epsg = atoi(optarg);
//           break;

//         case 'f':
//           printf ("option -f (export filename) with value `%s'\n", optarg);
//           epsg_convert_types.push_back(phreesqlib::DBEngine::EPSG_CONVERT_TYPE::CSV);
//           epsg_convert_outputs.push_back(optarg);          break;

//         case 'F':
//           printf ("option -F (export folder) with value `%s'\n", optarg);
//           export_folder = optarg;
//           break;

//         case 'i':
//           printf ("option -i (input folder) with value `%s'\n", optarg);
//           in_folder = optarg;
//           break;

//         case 'I':
//           printf ("option -I (export single analysis) with value `%s'\n", optarg);
//           export_analysis_id = atoi(optarg);
//           break;

//         case 'L':
//           printf ("option -I (export list of analysis) with value `%s'\n", optarg);
//           export_ids_list_filename = optarg;
//           break;

//         case 'o':
//           printf ("option -o (output folder) with value `%s'\n", optarg);
//           out_folder = optarg;
//           break;

//         case 'm':
//           printf ("option -m (meta folder) with value `%s'\n", optarg);
//           meta_folder = optarg;
//           break;

//         case 'P':
//           printf ("option -P (Phreeqc DB) with value `%s'\n", optarg);
//           phreeqc_db_path = optarg;
//           break;

//         case 't':
//           printf ("option -t (export table) with value `%s'\n", optarg);
//           epsg_convert_types.push_back(phreesqlib::DBEngine::EPSG_CONVERT_TYPE::TABLE);
//           epsg_convert_outputs.push_back(optarg);
//           break;

//         default:
//           abort ();
//         }
//     }

    if (export_all > 0)
    {
        export_input = 1;
        export_output = 1;
        export_metadata = 1;
    }

    if (fill_db > 0)
    {
        bool error = false;

        if (in_folder.length() == 0 || out_folder.length() == 0 || meta_folder.length() == 0)
        {
            std::cerr << "error - in, out and meta folders must be provided as an input." << std::endl;
            error = true;
        }

        if (db.length() == 0)
        {
            std::cerr << "error - database parameter missing" << std::endl;
            error = true;
        }

        if (error) return 1;
    }

    if ((export_input > 0 || export_output > 0 || export_metadata > 0))
    {
        bool error = false;

        if (export_folder.length() == 0)
        {
            std::cerr << "error - export folder must be provided to perform export operations." << std::endl;
            error = true;
        }

        if (db.length() == 0)
        {
            std::cerr << "error - database parameter missing" << std::endl;
            error = true;
        }

        if (error) return 1;
    }

    if (epsg_convert > 0)
    {
        bool error = false;

        if (epsg == INT_MAX)
        {
            std::cerr << "error - epsg parameter missing" << std::endl;
            error = true;
        }

        if (db.length() == 0)
        {
            std::cerr << "error - database parameter missing" << std::endl;
            error = true;
        }

        if (epsg_convert_types.empty())
        {
            std::cerr << "error - epsg conversion output parameter missing" << std::endl;
            error = true;
        }

        if (error) return 1;
    }

    if (run_phreeqc > 0 )
    {
        bool error = false;

        if (in_folder.length() == 0)
        {
            std::cerr << "error - PhreeQC input folder must be provided to run PhreeQC. " << std::endl;
            error = true;
        }

        if ( out_folder.length() == 0)
        {
            std::cerr << "error - PhreeQC output folder must be provided to run PhreeQC. " << std::endl;
            error = true;
        }

        if (phreeqc_db_path.length()==0 )
        {
            std::cerr << "error - PhreeQC DB must be provided to run PhreeQC. " << std::endl;
            error = true;
        }

        if (error)
            return 1;
    }

    phreesqlib::PhreeSQLibEngine engine (db);

    if (run_phreeqc > 0)
    {
        std::cout << std::endl;
        std::cout << "========================================================================================" << std::endl;
        std::cout << std::endl << "Running PhreeQC using DB " << phreeqc_db_path << "..." << std::endl << std::endl;
        engine.run_phreeqc_on_folder(in_folder, out_folder, phreeqc_db_path);
    }

    if (fill_db > 0)
    {
        std::cout << std::endl;
        std::cout << "========================================================================================" << std::endl;
        std::cout << "Reading folders and filling database " << db << "..." << std::endl;

        engine.run_on_folder(in_folder, out_folder, meta_folder);
    }

    if (epsg_convert > 0)
    {
        std::cout << std::endl;
        std::cout << "========================================================================================" << std::endl;
        std::cout << "EPSG Routine Running ..." << std::endl;

        std::string filename = db + "_epsg" + std::to_string(epsg) + ".csv";
        engine.epsg_convert (epsg, epsg_convert_types, epsg_convert_outputs);
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
            engine.export_input(export_folder, ids, overwrite);
        }

        if (export_output > 0)
        {
            std::cout << "========================================================================================" << std::endl;
            std::cout << "Exporting outputs from " << db << "..." << std::endl;
            engine.export_output(export_folder, ids, overwrite);
        }

        if (export_metadata > 0)
        {
            std::cout << "========================================================================================" << std::endl;
            std::cout << "Exporting metadata from " << db << "..." << std::endl;
            engine.export_metadata(export_folder, ids, overwrite);
        }


    }

    return 0;
}
