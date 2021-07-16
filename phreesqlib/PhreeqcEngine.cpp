#include "PhreeqcEngine.h"

#include <dirent.h>
#include <iostream>

/**
 * @brief phreesqlib::PhreeqcEngine::run_on_folder
 * @param foldername the input folder name
 * @return input/output mapping
 */
inline
std::vector<phreesqlib::PhreeqcEngineObj> phreesqlib::PhreeqcEngine::run_on_folder(const std::string & foldername)
{
    struct dirent *entry;
    DIR *dir = opendir(foldername.c_str());

    std::vector<PhreeqcEngineObj> results;

    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            results.push_back(run_on_file(entry->d_name, std::string(entry->d_name) + ".pqo"));
        }

        closedir(dir);
    }

    return results;
}

/**
 * @brief phreesqlib::PhreeqcEngine::run_on_file
 * @param in_filename the input filename
 * @param out_filename the output filename
 * @return input/output mapping with error code returned by phreeqc
 */
inline
phreesqlib::PhreeqcEngineObj phreesqlib::PhreeqcEngine::run_on_file(const std::string & in_filename,
                                                                    const std::string & out_filename)
{
    IPhreeqc pqc;
    pqc.SetOutputFileOn(true);
    pqc.SetOutputFileName(out_filename.c_str());

    PhreeqcEngineObj obj;

    obj.set_in_filename(in_filename);
    obj.set_out_filename(out_filename);

    obj.set_error_counter( pqc.RunFile(in_filename.c_str()) );

    return obj;
}


