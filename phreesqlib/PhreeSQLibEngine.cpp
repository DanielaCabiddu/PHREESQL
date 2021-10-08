#include "PhreeSQLibEngine.h"

#include "PhreeqcEngine.h"
#include "DBEngine.h"

#include <limits.h>

/**
 * @brief phreesqlib::PhreeSQLibEngine::run_on_folder
 * @param folder
 */
void phreesqlib::PhreeSQLibEngine::run_on_folder (const std::string in_folder,
                                                  const std::string out_folder,
                                                  const std::string meta_folder)
{
     std::string separator;

#ifdef _WIN32
    separator = "\\";
#else
    separator = "/";
#endif

//    phreesqlib::PhreeqcEngine *pqc_engine = new phreesqlib::PhreeqcEngine ();
//    results = pqc_engine->run_on_folder(in_folder, out_folder);

//    delete pqc_engine;

    phreesqlib::DBEngine *db_engine = new phreesqlib::DBEngine (db_filename);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (in_folder.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
          const std::string filename = ent->d_name;

          if (filename.compare("..") == 0) continue;
          if (filename.compare(".") == 0) continue;

          const uint ext_id = filename.find_last_of(".");
          const std::string ext = (ext_id < UINT_MAX) ? filename.substr(ext_id) : "";

          if (ext.compare(".pqi") != 0)
          {
              std::cerr << ent->d_name << " ]] Invalid Input File. IGNORED" << std::endl;
              continue;
          }

          const std::string basename = (ext_id < UINT_MAX) ? filename.substr(0, ext_id) : filename;

          std::cout << ent->d_name << "(" << basename << ", " << ext << ")" << std::endl;

          const std::string out_filename = out_folder + separator + basename + ".pqo";
          const std::string meta_filename = meta_folder + separator + basename + ".meta";

          struct stat buffer_out, buffer_meta;
          if (stat (out_filename.c_str(), &buffer_out) != 0)
          {
              std::cerr << ent->d_name << " ]] No Matching OUT File. IGNORED." << std::endl;
              continue;
          }

          if (stat (meta_filename.c_str(), &buffer_meta) != 0)
          {
              std::cerr << ent->d_name << " ]] No Matching META File. IGNORED." << std::endl;
              continue;
          }


          phreesqlib::PhreeqcEngineObj obj;
          obj.set_in_filename(in_folder + separator + filename);
          obj.set_out_filename(out_filename);

          db_engine->add_to_DB(obj, meta_filename);
        }



        closedir (dir);
    }
    else
    {
      /* could not open directory */
        std::string error = std::string("Could NOT Open Input Folder ") + in_folder;
        perror (error.c_str());
        return ;
    }

    delete db_engine;
}
