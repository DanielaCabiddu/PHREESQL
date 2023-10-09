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

#include "ProJEngine.h"

#include <iostream>
#include <math.h>
#include <string>

inline
void phreesqlib::ProjEngine::epsg2epsg (const double epsg1_x, 
                                        const double epsg1_y, 
                                        const double epsg1_z, 
                                        const unsigned int epsg1, 
                                        const unsigned int epsg2,
                                        double &epsg2_x, 
                                        double &epsg2_y, 
                                        double &epsg2_z )
{

    PJ_CONTEXT *c = proj_context_create();
    PJ *transformation = proj_create_crs_to_crs(c,
                                                std::string("EPSG:" + std::to_string(epsg1)).c_str(),
                                                std::string("EPSG:" + std::to_string(epsg2)).c_str(),
                                                nullptr);

    epsg2epsg(transformation, epsg1_x, epsg1_y, epsg1_z, epsg2_x, epsg2_y, epsg2_z);

    proj_context_destroy (c);
}

inline
void phreesqlib::ProjEngine::epsg2epsg (PJ *transformation, 
                                        const double epsg1_x, 
                                        const double epsg1_y, 
                                        const double epsg1_z, 
                                        double &epsg2_x, 
                                        double &epsg2_y, 
                                        double &epsg2_z )
{

    if (!transformation)
    {
        std::cerr << "ERROR PROJ" << std::endl;

        ////// to be handled in some way
        exit(1);
    }

    PJ_COORD coords;
    coords.xyz.x = epsg1_x;
    coords.xyz.y = epsg1_y;
    coords.xyz.z = epsg1_z;

//    std::cout << "DEGREE : " << coords.lpz.phi << " - " << coords.lpz.lam << " - " << coords.lpz.z << std::endl;

    PJ_COORD converted_coords = proj_trans(transformation, PJ_DIRECTION::PJ_FWD, coords);

//    std::cout << "CONVERTED : " << converted_coords.xyz/.x << " - " << converted_coords.xyz.y << " - " << converted_coords.xyz.z << std::endl;

    epsg2_x = converted_coords.xyz.x;
    epsg2_y = converted_coords.xyz.y;
    epsg2_z = converted_coords.xyz.z;
}
