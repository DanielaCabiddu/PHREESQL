#include "PhreeqcEngine.h"
#include "DBEngine.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

using namespace std;

int main(int argc, char *argv[])
{
    std::string file;
    std::string folder;
    std::string db;

    while (1)
    {
        static struct option long_options[] =
        {
          /* These options set a flag. */
    //      {"verbose", no_argument,       &verbose_flag, 1},
    //      {"brief",   no_argument,       &verbose_flag, 0},
          /* These options don’t set a flag.
             We distinguish them by their indices. */
    //      {"add",     no_argument,       0, 'a'},
          {"database",  required_argument, 0, 'd'},
          {"file",      required_argument, 0, 'f'},
          {"folder",    required_argument, 0, 'F'},
          {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        int c = getopt_long (argc, argv, "d:f:F:", long_options, &option_index);

        if (c==-1) break;

        switch (c)
        {
//        case 0:
          /* If this option set a flag, do nothing else now. */
//                  if (long_options[option_index].flag != 0)
//                    break;
//                  printf ("option %s", long_options[option_index].name);
//                  if (optarg)
//                    printf (" with arg %s", optarg);
//                  printf ("\n");
//                  break;

        case 'd':
          printf ("option -d with value `%s'\n", optarg);
          db = optarg;
          break;

        case 'f':
          printf ("option -f with value `%s'\n", optarg);
          file = optarg;
          break;

        case 'F':
          printf ("option -F with value `%s'\n", optarg);
          folder = optarg;
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

    if (file.length() == 0 && folder.length() == 0)
    {
        std::cerr << "error - at least one between folder and file parameter must be set" << std::endl;
        return 1;
    }

    phreesqlib::PhreeqcEngine pqc_engine;
    std::vector<phreesqlib::PhreeqcEngineObj> results;

    if (folder.length() > 0)
        results = pqc_engine.run_on_folder(folder);

    if (file.length() > 0)
        results.push_back(pqc_engine.run_on_file(file, file + ".pqo"));

    phreesqlib::DBEngine db_engine (db);

    for (const phreesqlib::PhreeqcEngineObj & obj : results)
    {
        if (obj.get_error_counter() > 0)
        {
            std::cerr << "Some error occured while processing " << obj.get_in_filename()
                      << ". IGNORED. " << std::endl;
            continue;
        }

        db_engine.add_to_DB(obj);
    }

    return 0;
}
