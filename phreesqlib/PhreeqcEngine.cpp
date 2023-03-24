#include "PhreeqcEngine.h"

#include <dirent.h>
#include <iostream>

inline
std::vector<phreesqlib::PhreeqcEngineObj> phreesqlib::PhreeqcEngine::run_on_folder(const std::string & in_foldername,
                                                                                   const std::string & out_foldername)
{
    struct dirent *entry;
    DIR *dir = opendir(in_foldername.c_str());

    std::vector<PhreeqcEngineObj> results;

    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
// get corresponding out name
            results.push_back(run_on_file(entry->d_name, std::string(entry->d_name) + ".pqo"));
        }

        closedir(dir);
    }

    return results;
}

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


