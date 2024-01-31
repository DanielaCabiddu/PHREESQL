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

#ifndef PHREESQLIB_INPUT_DB_MANAGER
#define PHREESQLIB_INPUT_DB_MANAGER

#include "db-manager.h"

class InputDBManager : public DBManager
{

public:
    ///
    /// \brief InputDBManager
    /// \param db
    ///
    InputDBManager(sqlite3 *db) : DBManager(db) {}

    ~InputDBManager () {}

    ///
    /// \brief isInputEmpty
    /// \return
    ///
    bool isInputEmpty();

    ///
    /// \brief createSolutionInputTable
    ///
    void createSolutionInputTable();

    ///
    /// \brief insertSolutionInputs
    /// \param inputs
    /// \param a_id
    ///
    void insertSolutionInputs(const vector<SolutionInput> &inputs, int a_id);

    ///
    /// \brief selectValuesFromMetadata
    /// \param meta
    /// \param ANALYSIS_num
    ///
    void selectValuesFromMetadata(metadata &meta, int ANALYSIS_num);

    ///
    /// \brief selectValuesFromSolution
    /// \param inputs
    /// \param ANALYSIS_num
    ///
    void selectValuesFromSolution(vector<SolutionInput> &inputs, int ANALYSIS_num);
};

#ifndef PHREESQL_STATIC
#include "input-db-manager.cpp"
#endif

#endif

