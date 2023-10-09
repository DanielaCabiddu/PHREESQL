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

#ifndef PHREESQLIB_DATA_MANAGER
#define PHREESQLIB_DATA_MANAGER

#include "db-manager.h"

#include <string>
#include <vector>
#include <float.h>
#include <limits.h>
#include "utils.h"

class DataManager : public DBManager
{
public:
    ///
    /// \brief DataManager
    /// \param db
    ///
    DataManager(sqlite3 *db) : DBManager(db) {}

    ~DataManager () {}

    ///
    /// \brief getMetadata
    /// \return
    ///
    std::vector<std::vector<std::pair<std::string, std::string>>> getMetadata();

    ///
    /// \brief updateMetadataEPSG
    /// \param epsg
    /// \param id
    /// \param x
    /// \param y
    /// \return
    ///
    bool updateMetadataEPSG (const unsigned int epsg,
                             const std::vector<std::string> &id,
                             const std::vector<double> &x,
                             const std::vector<double> &y);

    ///
    /// \brief getData
    /// \param table_name
    /// \param value
    /// \param column_name
    /// \param timestamp
    /// \return
    ///
    vector<vector<string> > getData(string table_name, string value, string column_name, string timestamp);

    ///
    /// \brief writeToFile
    /// \param path
    /// \param data
    /// \return
    ///
    bool writeToFile(string path, vector<vector<string> > data);
};

#ifndef PHREESQL_STATIC
#include "data-manager.cpp"
#endif

#endif
