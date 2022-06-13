#include "ProJEngine.h"

#include <iostream>
#include <math.h>
#include <string>

void ProjEngine::epsg2epsg (const double epsg1_x, const double epsg1_y, const double epsg1_z, const unsigned int epsg1, const unsigned int epsg2,
                 double &epsg2_x, double &epsg2_y, double &epsg2_z )
{

    PJ_CONTEXT *c = proj_context_create();
    PJ *transformation = proj_create_crs_to_crs(c,
                                                std::string("EPSG:" + std::to_string(epsg1)).c_str(),
                                                std::string("EPSG:" + std::to_string(epsg2)).c_str(),
                                                nullptr);

    epsg2epsg(transformation, epsg1_x, epsg1_y, epsg1_z, epsg2_x, epsg2_y, epsg2_z);

    proj_context_destroy (c);
}

void epsg2epsg (PJ *transformation, const double epsg1_x, const double epsg1_y, const double epsg1_z, double &epsg2_x, double &epsg2_y, double &epsg2_z )
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
