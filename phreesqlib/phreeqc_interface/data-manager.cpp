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

#include "data-manager.h"
#include "db-manager.h"

#include <string>
#include <vector>
#include <float.h>
#include <limits.h>
#include "utils.h"

inline
std::vector<std::vector<std::pair<std::string, std::string>>> DataManager::getMetadata()
{
    int row, column;
    std::vector<std::vector<std::pair<std::string, std::string>>> res;

    std::string query = "SELECT * FROM " + metadata_table_name + ";";
    std::cout << query << std::endl;

    int rc = sqlite3_exec(db, query.c_str(), printDBCallback, &res, &err_message);
    this->queryResult(rc, "data2epsg");

    for (uint i=0; i < res.size(); i++)
    {
        //std::cout << "[" << i << "]" << res.at(i).size() << std::endl;

        for (const std::pair<std::string, std::string> &p : res.at(i))
        {
            //std::cout << p.first << " : " << p.second << std::endl;
        }
    }

    return res;
}

inline
bool DataManager::updateMetadataEPSG (const uint epsg,
                         const std::vector<std::string> &id,
                         const std::vector<double> &x,
                         const std::vector<double> &y)
{
    for (uint i=0; i < id.size(); i++)
    {
        query = "UPDATE " + metadata_table_name + " " +
                "SET EPSG = '" + to_string(epsg) + "', " +
                     "COORD_X = '" + to_string(x.at(i)) + "', " +
                     "COORD_Y = '" + to_string(y.at(i)) + "' " +
                "WHERE ID = '" + id.at(i) + "'";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

        if (rc == SQLITE_OK)
        {
//                this->queryResult(rc, "inserting epsg");
        }
        else
        {
            std::cerr << "\033[1;31mSQL ERROR " << rc << ": " << err_message << "\033[0m" << std::endl;
        }

    }

    return (rc == SQLITE_OK);
}

inline
vector<vector<string> > DataManager::getData(string table_name, string value, string column_name, string timestamp)
{
    char **res;
    int row, column;

    query = "SELECT " + toUpperString(column_name) + ", TIMESTAMP FROM " + toUpperString(table_name) + " JOIN ANALISYS ON ANALISYS_ID LIKE ANALISYS.ID WHERE " + "ANALISYS.TIMESTAMP>=\"" + timestamp + "\" AND " + valueColName(table_name) + "=\"" + value + "\" ORDER BY TIMESTAMP;";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "getData");

    vector<vector<string> > data;
    vector<string> elems;
    for (int i = 2; i < (row + 1) * column; i = i + 2)
    {
        elems.clear();
        elems.push_back(res[i]);
        elems.push_back(res[i + 1]);
        data.push_back(elems);
    };

    sqlite3_free_table(res);
    return data;
}

bool writeToFile(string path, vector<vector<string> > data)
{
    fstream output_file;
    output_file.open(path, ios::out | ios::trunc);
    if (output_file.is_open())
    {
        for (int i = 0; i < data.size(); i++)
        {
            output_file << data[i][1] << " " << data[i][0] << endl;
        }
        return true;
    }

    return false;
}
