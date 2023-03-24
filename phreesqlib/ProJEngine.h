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

#ifndef _PROJ_ENGINE
#define _PROJ_ENGINE

#include <proj.h>

namespace phreesqlib
{
class ProjEngine
{

public:

    void epsg2epsg (const double epsg1_x, const double epsg1_y, const double epsg1_z, const unsigned int epsg1, const unsigned int epsg2,
                     double &epsg2_x, double &epsg2_y, double &epsg2_z );

    void epsg2epsg (PJ *transformation, const double epsg1_x, const double epsg1_y, const double epsg1_z, double &epsg2_x, double &epsg2_y, double &epsg2_z );
};
}

#ifndef PHREESQL_STATIC
#include "ProJEngine.cpp"
#endif

#endif
