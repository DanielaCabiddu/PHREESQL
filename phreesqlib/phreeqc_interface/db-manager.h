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

#ifndef PHREESQLIB_DB_MANAGER
#define PHREESQLIB_DB_MANAGER

#include <sqlite3.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "models.h"

//const char *DB_NAME = "matrac.db";
#ifndef DBM
#define DBM
#pragma once

static int count_rows(void *param, int argc, char **argv, char **azColName);

static int emptyDBCallback(void *param, int argc, char **argv, char **azColName);

static int printDBCallback(void *list, int count, char **data, char **columns);

class DBManager
{
protected:

    ///
    /// \brief db
    ///
    sqlite3 *db;

    ///
    /// \brief err_message
    ///
    char *err_message;

    ///
    /// \brief rc
    ///
    int rc;

    ///
    /// \brief query
    ///
    string query;


public:

    ///
    /// \brief metadata_table_name
    ///
    const string metadata_table_name = "METADATA";

    ///
    /// \brief DBManager
    /// \param db
    ///
    DBManager(sqlite3 *db);

    ~DBManager () {}

    ///
    /// \brief queryResult
    /// \param rc
    /// \param message
    ///
    void queryResult(int rc, string message);

    ///
    /// \brief isDBEmpty
    /// \return
    ///
    bool isDBEmpty();

    ///
    /// \brief getNumRows
    /// \param table_name
    /// \return
    ///
    int getNumRows(string table_name);
};

#ifndef PHREESQL_STATIC
#include "db-manager.cpp"
#endif

#endif
#endif
