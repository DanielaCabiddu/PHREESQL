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

#ifndef _NMEA
#define _NMEA

#ifdef USE_PROJ
#include <proj.h>
#endif

double nmea2degrees (const double nmea_val);

#ifdef USE_PROJ
void nmea2world (const double nmea_x, const double nmea_y, const double nmea_z, const unsigned int nmea_epsg, const unsigned int world_epsg,
                 double &world_x, double &world_y, double &world_z );

void nmea2world (PJ *transformation, const double nmea_x, const double nmea_y, const double nmea_z, double &world_x, double &world_y, double &world_z );

void world2nmea (const double world_x, const double world_y, const double world_z, const unsigned int world_epsg, const unsigned int nmea_epsg,
                 double &nmea_x, double &nmea_y, double &nmea_z );

void world2degree_nmea (const double world_x, const double world_y, const double world_z, const unsigned int world_epsg, const unsigned int nmea_epsg,
                 double &nmea_x, double &nmea_y, double &nmea_z );
#endif

#ifndef STATIC_LIB
#include "nmea.cpp"
#endif


#endif
