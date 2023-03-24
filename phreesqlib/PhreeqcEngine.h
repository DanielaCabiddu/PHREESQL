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

#ifndef PHREEQC_ENGINE
#define PHREEQC_ENGINE

#include "PhreeqcEngineObj.h"
#include <IPhreeqc.hpp>

#include <string>

namespace phreesqlib
{
///
/// \brief The PhreeqcEngine class
///
class PhreeqcEngine
{
public:

    ///
    /// \brief PhreeqcEngine
    ///
    PhreeqcEngine () {}

    ///
    /// \brief run_on_folder
    /// \param in_foldername
    /// \param out_foldername
    /// \return
    ///
    std::vector<PhreeqcEngineObj> run_on_folder (const std::string & in_foldername,
                                                 const std::string & out_foldername) ;

    ///
    /// \brief run_on_file
    /// \param in_filename
    /// \param out_filename
    /// \return
    ///
    PhreeqcEngineObj run_on_file(const std::string &in_filename, const std::string &out_filename) ;
};

}

#ifndef PHREESQL_STATIC
#include "PhreeqcEngine.cpp"
#endif

#endif
