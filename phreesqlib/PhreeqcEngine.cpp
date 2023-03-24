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


