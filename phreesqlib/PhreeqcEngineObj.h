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

#ifndef PHREEQCOBJ_ENGINE
#define PHREEQCOBJ_ENGINE

#include <string>

namespace phreesqlib
{

///
/// \brief The PhreeqcEngineObj class
///
class PhreeqcEngineObj
{
public:
    ///
    /// \brief PhreeqcEngineObj
    ///
    PhreeqcEngineObj () {}

    ///
    /// \brief set_error_counter
    /// \param i
    ///
    void set_error_counter  (const unsigned int i)  { error_counter = i; }

    ///
    /// \brief set_in_filename
    /// \param s
    ///
    void set_in_filename    (const std::string s)   { in_filename = s; }

    ///
    /// \brief set_out_filename
    /// \param s
    ///
    void set_out_filename   (const std::string s)   { out_filename = s; }

    ///
    /// \brief get_error_counter
    /// \return
    ///
    unsigned int get_error_counter  () const { return error_counter; }

    ///
    /// \brief get_in_filename
    /// \return
    ///
    std::string  get_in_filename    () const { return in_filename; }

    ///
    /// \brief get_out_filename
    /// \return
    ///
    std::string  get_out_filename   () const { return out_filename; }

private:

    ///
    /// \brief error_counter
    ///
    unsigned int error_counter = 0;

    ///
    /// \brief in_filename
    ///
    std::string in_filename;

    ///
    /// \brief out_filename
    ///
    std::string out_filename;
};


}

//#ifndef PHREESQL_STATIC
//#include "PhreeqcEngine.cpp"
//#endif

#endif
