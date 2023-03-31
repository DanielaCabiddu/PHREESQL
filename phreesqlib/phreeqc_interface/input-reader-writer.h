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

#ifndef PHREESQLIB_READER_WRITER
#define PHREESQLIB_READER_WRITER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "models.h"
#include "utils.h"
using namespace std;

///
/// \brief The InputReaderWriter class
///
class InputReaderWriter
{
public:
    ///
    /// \brief input_list
    ///
    vector<SolutionInput> input_list;

    ///
    /// \brief meta
    ///
    metadata meta;

    ///
    /// \brief InputReaderWriter
    ///
    InputReaderWriter() {}

    ///
    /// \brief readFile
    /// \param path
    /// \return
    ///
    bool readFile(string path);

    ///
    /// \brief writeFile
    /// \param i
    /// \param file_path
    /// \return
    ///
    bool writeFile(int i, string file_path);
};

#ifndef PHREESQL_STATIC
#include "input-reader-writer.cpp"
#endif

#endif
