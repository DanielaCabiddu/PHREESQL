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

#include "input-db-manager.h"

inline
bool InputDBManager::isInputEmpty()
{
    int num_tables;
    query = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='SOLUTION_INPUT';";
    rc = sqlite3_exec(db, query.c_str(), emptyDBCallback, &num_tables, &err_message);
    this->queryResult(rc, "isInputEmpty");
    return num_tables == 0;
}

inline
void InputDBManager::createSolutionInputTable()
{
    this->err_message = 0;
    query = "CREATE TABLE IF NOT EXISTS SOLUTION_INPUT "
            "( ID INTEGER PRIMARY KEY NOT NULL,"
            "ANALYSIS_ID INTEGER NOT NULL,"
            "SOLUTION_NUMBER TEXT,"
            "PARAM TEXT, VALUE TEXT,"
            "CONSTRAINT ANALYSIS_ID "
            "FOREIGN KEY (ANALYSIS_ID) REFERENCES " + metadata_table_name + "(ID) );";
    rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

    this->queryResult(rc, "Create table solution_input");
}

inline
void InputDBManager::insertSolutionInputs(vector<SolutionInput> inputs, int a_id)
{
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    for (int i = 0; i < inputs.size(); i++)
    {
        query = "INSERT OR REPLACE INTO SOLUTION_INPUT (ANALYSIS_ID, SOLUTION_NUMBER, PARAM, VALUE) VALUES ('" +
                to_string(a_id) + "', '" +
                inputs[i].solution_number + "', '" +
                inputs[i].unit + "', '" +
                inputs[i].ppm + "');";

        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
        this->queryResult(rc, "insert or replace solution_input");
    }

    sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
}

inline
void InputDBManager::selectValuesFromMetadata(metadata &meta, int ANALYSIS_num)
{
    char **res;
    int row, column;

    meta.clear();

    query = "SELECT * FROM " + metadata_table_name + " WHERE ID='" + to_string(ANALYSIS_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selecting data from metadata");

    for (int i = 0; i < column; i++)
    {
        meta.insert(pair<string, string>(res[i], res[i + column]));
    }
    sqlite3_free_table(res);
}

inline
void InputDBManager::selectValuesFromSolution(vector<SolutionInput> &inputs, int ANALYSIS_num)
{
    char **res;
    int row, column;
    SolutionInput input;

    query = "SELECT * FROM SOLUTION_INPUT WHERE ANALYSIS_ID='" + to_string(ANALYSIS_num) + "';";
    rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
    this->queryResult(rc, "selecting data from solution_input");

    for (int i = 5; i < (row + 1) * column; i = i + 5)
    {
        input.analisys_id = atoi(res[i + 1]);
        input.solution_number = res[i + 2];
        input.unit = res[i + 3];
        input.ppm = res[i + 4];
        inputs.push_back(input);
    }
    sqlite3_free_table(res);
}
