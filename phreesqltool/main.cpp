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
          {"database",      required_argument, 0, 'd'},
          {"in_folder",     required_argument, 0, 'I'},
          {"out_folder",    required_argument, 0, 'O'},
          {"meta_folder",   required_argument, 0, 'M'},
          {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        int c = getopt_long (argc, argv, "d:I:O:M:", long_options, &option_index);

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

//        case 'f':
//          printf ("option -f with value `%s'\n", optarg);
//          file = optarg;
//          break;

        case 'I':
          printf ("option -I with value `%s'\n", optarg);
          in_folder = optarg;
          break;

        case 'O':
          printf ("option -O with value `%s'\n", optarg);
          out_folder = optarg;
          break;

        case 'M':
          printf ("option -M with value `%s'\n", optarg);
          meta_folder = optarg;
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

    if (in_folder.length() == 0 || out_folder.length() == 0 || meta_folder.length() == 0)
    {
        std::cerr << "error - in, out and meta folders must be provided as an input." << std::endl;
        return 1;
    }

    phreesqlib::PhreeSQLibEngine engine (db);

    if (in_folder.length() > 0)
        engine.run_on_folder(in_folder, out_folder, meta_folder);

    return 0;
}
