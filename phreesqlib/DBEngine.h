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

#ifndef DB_ENGINE
#define DB_ENGINE

#include "PhreeqcEngine.h"

#include "phreeqc_interface/matrac-reader.h"

namespace phreesqlib
{

///
/// \brief The DBEngine class
///
class DBEngine
{
public:
    ///
    /// \brief DBEngine
    /// \param filename
    ///
    DBEngine (const std::string filename) {
        db_filename = filename;
    }

    ~DBEngine ()
    {
    }

    ///
    /// \brief The EPSG_CONVERT_TYPE enum
    ///
    /// \details The EPSG_CONVERT_TYPE enum enables the possibility to specify how to save the output of an EPSG conversion.
    ///
    enum EPSG_CONVERT_TYPE
    {
        CSV,    /// As a CSV file
        TABLE,  /// As a new table of the input PHREESQL database
        DB,     /// As a new database which is a copy of the input one with converted EPSG
        UNSET,  /// Unknown.
    };

    ///
    /// \brief add_to_DB
    /// \param obj
    /// \param metadata_filename
    ///
    void add_to_DB(const PhreeqcEngineObj &obj, const std::string metadata_filename);

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
    /// \brief convert_epsg
    /// \param epsg
    /// \param types
    /// \param outputs
    ///
    void convert_epsg (const int epsg, const std::vector<EPSG_CONVERT_TYPE> types, std::vector<string> outputs);

    ///
    /// \brief print_DB_summary
    ///
    void print_DB_summary () const;

private:

    ///
    /// \brief db_filename
    ///
    std::string db_filename;

};

}

#ifndef PHREESQL_STATIC
#include "DBEngine.cpp"
#endif

#endif
