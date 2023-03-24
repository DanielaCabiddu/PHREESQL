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

#include <sqlite3.h>
#include <algorithm>
#include <iterator>
#include <fstream>
#include "models.h"
using namespace std;

//const char *DB_NAME = "matrac.db";
#ifndef DBM
#define DBM
#pragma once

static int count_rows(void *param, int argc, char **argv, char **azColName)
{
    int *value = static_cast<int *>(param);

    *value = atoi(argv[0]);

    return 0;
}

static int emptyDBCallback(void *param, int argc, char **argv, char **azColName)
{
    int *value = static_cast<int *>(param);

    *value = atoi(argv[0]);

    return 0;
}

static int printDBCallback(void *list, int count, char **data, char **columns)
{
    int idx;

    std::vector<std::vector<std::pair<std::string, std::string>>> *res_list =
            (std::vector<std::vector<std::pair<std::string, std::string>>> *) list;

    std::vector<std::pair<std::string, std::string>> record;

    for (idx = 0; idx < count; idx++) {
//        printf("The data in column \"%s\" is: %s\n", columns[idx], data[idx]);
         record.push_back(std::pair<std::string, std::string> (columns[idx], data[idx]));
    }

    res_list->push_back(record);

    return 0;
}

class DBManager
{
protected:
    sqlite3 *db;
    char *err_message;
    int rc;
    string query;


public:

    const string metadata_table_name = "METADATA";

    DBManager(sqlite3 *db)
    {
        this->db = db;
    }

    ~DBManager () {}

    void queryResult(int rc, string message)
    {
        this->err_message = 0;
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error on %s: %s (%s)\n", message.c_str(), err_message, sqlite3_errstr(rc));
            sqlite3_free(err_message);
        }
        else
        {
            //fprintf(stdout, message.c_str());
        }
    }

    bool isDBEmpty()
    {
        int num_tables;
        query = "SELECT count(*) FROM sqlite_master WHERE type='table';";
        rc = sqlite3_exec(db, query.c_str(), emptyDBCallback, &num_tables, &err_message);
        this->queryResult(rc, "isDBEmpty");
        return num_tables == 0;
    }

    int getNumRows(string table_name)
    {
        int num;
        query = "SELECT count(*) FROM " + table_name + ";";
        rc = sqlite3_exec(db, query.c_str(), count_rows, &num, &err_message);
        return num;
    }
};

#endif
