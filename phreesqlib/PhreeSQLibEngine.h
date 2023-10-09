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

#ifndef PHREESQLIB_ENGINE
#define PHREESQLIB_ENGINE

#include "DBEngine.h"
#include <string>
#include <vector>

/*! \file
 *  \brief PhreeSQLib Engine
 */

namespace phreesqlib
{

///
/// \brief The PhreeSQLibEngine class
///
class PhreeSQLibEngine
{
public:
    ///
    /// \brief PhreeSQLibEngine
    /// \param db
    ///
    PhreeSQLibEngine (const std::string db) { db_filename=db; }

    ///
    /// \brief run_on_folder
    /// \param in_folder
    /// \param out_folder
    /// \param meta_folder
    ///
    void run_on_folder (const std::string in_folder, const std::string out_folder, const std::string meta_folder);

    ///
    /// \brief run_phreeqc_on_folder
    /// \param in_folder
    /// \param out_folder
    /// \param phreeqc_db_path
    ///
    void run_phreeqc_on_folder (const std::string in_folder, const std::string out_folder, const std::string phreeqc_db_path);

    ///
    /// \brief export_input
    /// \param out_folder
    /// \param analysis_ids
    /// \param overwrite
    ///
    void export_input (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);

    ///
    /// \brief export_output
    /// \param out_folder
    /// \param analysis_ids
    /// \param overwrite
    ///
    void export_output (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);

    ///
    /// \brief export_metadata
    /// \param out_folder
    /// \param analysis_ids
    /// \param overwrite
    ///
    void export_metadata (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite = true);

    ///
    /// \brief epsg_convert
    /// \param epsg
    /// \param types
    /// \param outputs
    ///
    void epsg_convert (const int epsg, const std::vector<EPSG_CONVERT_TYPE> types, std::vector<std::string> outputs);

private:

    ///
    /// \brief db_filename
    ///
    std::string db_filename;

    ///
    /// \brief in_ext
    ///
    const std::string in_ext = ".pqi";

    ///
    /// \brief out_ext
    ///
    const std::string out_ext = ".pqo";

    ///
    ////// \brief meta_ext
    ///
    const std::string meta_ext = ".met";
};

}

#ifndef PHREESQL_STATIC
#include "PhreeSQLibEngine.cpp"
#endif

#endif
