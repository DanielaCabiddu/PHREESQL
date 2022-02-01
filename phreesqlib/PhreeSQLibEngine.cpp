#include "PhreeSQLibEngine.h"

#include "PhreeqcEngineObj.h"
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

    unsigned int file_counter = 0;
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

          const unsigned int ext_id = filename.find_last_of(".");
          const std::string ext = (ext_id < UINT_MAX) ? filename.substr(ext_id) : "";

          if (ext.compare(in_ext) != 0)
          {
              std::cerr << ent->d_name << " ]] Invalid Input File. IGNORED" << std::endl;
              continue;
          }

          const std::string basename = (ext_id < UINT_MAX) ? filename.substr(0, ext_id) : filename;

          const std::string in_absolute_path = in_folder + separator + basename + in_ext;
          const std::string out_absolute_path = out_folder + separator + basename + out_ext;
          const std::string meta_absolute_path = meta_folder + separator + basename + meta_ext;

          std::cout << "[" << file_counter << "] Processing " << in_absolute_path << std::endl;
//          std::cout << " -- " << out_absolute_path << std::endl;
//          std::cout << " -- " << meta_absolute_path << std::endl;

          struct stat buffer_out, buffer_meta;
          if (stat (out_absolute_path.c_str(), &buffer_out) != 0)
          {
              std::cerr << ent->d_name << " ]] No Matching OUT File. IGNORED." << std::endl;
              continue;
          }

          if (stat (meta_absolute_path.c_str(), &buffer_meta) != 0)
          {
              std::cerr << ent->d_name << " ]] No Matching META File. IGNORED." << std::endl;
              continue;
          }


          phreesqlib::PhreeqcEngineObj obj;
          obj.set_in_filename(in_folder + separator + filename);
          obj.set_out_filename(out_absolute_path);

          db_engine->add_to_DB(obj, meta_absolute_path);

          file_counter++;

          std::cout << std::endl;
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

void phreesqlib::PhreeSQLibEngine::export_input (const std::string out_folder, const std::vector<int> analysis_ids)
{
    phreesqlib::DBEngine *db_engine = new phreesqlib::DBEngine (db_filename);
    db_engine->export_input(out_folder, analysis_ids);
    delete db_engine;
}

void phreesqlib::PhreeSQLibEngine::export_output (const std::string out_folder, const std::vector<int> analysis_ids)
{
    phreesqlib::DBEngine *db_engine = new phreesqlib::DBEngine (db_filename);

    db_engine->export_output(out_folder, analysis_ids);
    delete db_engine;
}
