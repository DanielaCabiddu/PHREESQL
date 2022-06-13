#include "nmea.h"

#include "../matrac_inline.h"

#include <iostream>
#include <math.h>
#include <string>

MATRAC_INLINE
double nmea2degrees (const double nmea_val)
{
    double d;
    std::modf(nmea_val / 100.0 , &d);
    double m = nmea_val - d*100.0;
    return (d + m/60.0 );
}

MATRAC_INLINE
double degrees2nmea (const double degrees_val)
{
    double num = std::abs(degrees_val);
    double d = double (floor(num));
    double m = num - d;

    m *= 60.0;

    double val = d*100 + m;

    if (degrees_val < 0)
        return  -val;

    return val;
}

#if USE_PROJ
MATRAC_INLINE
void nmea2world (const double nmea_x, const double nmea_y, const double nmea_z, const unsigned int nmea_epsg, const unsigned int world_epsg,
                 double &world_x, double &world_y, double &world_z )
{

    PJ_CONTEXT *c = proj_context_create();
    PJ *transformation = proj_create_crs_to_crs(c,
                                                std::string("EPSG:" + std::to_string(nmea_epsg)).c_str(),
                                                std::string("EPSG:" + std::to_string(world_epsg)).c_str(),
                                                nullptr);

    nmea2world(transformation, nmea_x, nmea_y, nmea_z, world_x, world_y, world_z);

    proj_context_destroy (c);
}

MATRAC_INLINE
void world2degree_nmea (const double world_x, const double world_y, const double world_z,
                 const unsigned int world_epsg, const unsigned int nmea_epsg,
                 double &nmea_x, double &nmea_y, double &nmea_z )
{
    PJ_CONTEXT *c = proj_context_create();
    PJ *transformation = proj_create_crs_to_crs(c,
                                                std::string("EPSG:" + std::to_string(world_epsg)).c_str(),
                                                std::string("EPSG:" + std::to_string(nmea_epsg)).c_str(),
                                                nullptr);

    if (!transformation)
    {
        std::cerr << "ERROR PROJ" << std::endl;

        ////// to be handled in some way
        exit(1);
    }

    PJ_COORD coords;
    coords.xyz.x = world_x;
    coords.xyz.y = world_y;
    coords.xyz.z = world_z;

//    std::cout << "DEGREE : " << coords.lpz.phi << " - " << coords.lpz.lam << " - " << coords.lpz.z << std::endl;

    PJ_COORD converted_coords = proj_trans(transformation, PJ_DIRECTION::PJ_FWD, coords);

   std::cout << "CONVERTED : " << converted_coords.xyz.x << " - " << converted_coords.xyz.y << " - " << converted_coords.xyz.z << std::endl;

//    nmea_x = degrees2nmea( converted_coords.lpz.lam );
//    nmea_y = degrees2nmea( converted_coords.lpz.phi );
//    nmea_z = converted_coords.lpz.z;

   nmea_x = converted_coords.xyz.x;
   nmea_y = converted_coords.xyz.y;
   nmea_z = converted_coords.xyz.z;

    proj_context_destroy (c);
}

MATRAC_INLINE
void world2nmea (const double world_x, const double world_y, const double world_z,
                 const unsigned int world_epsg, const unsigned int nmea_epsg,
                 double &nmea_x, double &nmea_y, double &nmea_z )
{
    PJ_CONTEXT *c = proj_context_create();
    PJ *transformation = proj_create_crs_to_crs(c,
                                                std::string("EPSG:" + std::to_string(world_epsg)).c_str(),
                                                std::string("EPSG:" + std::to_string(nmea_epsg)).c_str(),
                                                nullptr);

    if (!transformation)
    {
        std::cerr << "ERROR PROJ" << std::endl;

        ////// to be handled in some way
        exit(1);
    }

    PJ_COORD coords;
    coords.xyz.x = world_x;
    coords.xyz.y = world_y;
    coords.xyz.z = world_z;

//    std::cout << "DEGREE : " << coords.lpz.phi << " - " << coords.lpz.lam << " - " << coords.lpz.z << std::endl;

    PJ_COORD converted_coords = proj_trans(transformation, PJ_DIRECTION::PJ_FWD, coords);

//   std::cout << "CONVERTED : " << converted_coords.xyz.x << " - " << converted_coords.xyz.y << " - " << converted_coords.xyz.z << std::endl;

    nmea_x = degrees2nmea( converted_coords.lpz.lam );
    nmea_y = degrees2nmea( converted_coords.lpz.phi );
    nmea_z = converted_coords.lpz.z;

    proj_context_destroy (c);
}

MATRAC_INLINE
void nmea2world (PJ *transformation, const double nmea_x, const double nmea_y, const double nmea_z, double &world_x, double &world_y, double &world_z )
{

    if (!transformation)
    {
        std::cerr << "ERROR PROJ" << std::endl;

        ////// to be handled in some way
        exit(1);
    }

    PJ_COORD coords;
    coords.lpz.lam = nmea2degrees( nmea_x );
    coords.lpz.phi = nmea2degrees( nmea_y );
    coords.lpz.z = ( nmea_z );

//    std::cout << "DEGREE : " << coords.lpz.phi << " - " << coords.lpz.lam << " - " << coords.lpz.z << std::endl;

    PJ_COORD converted_coords = proj_trans(transformation, PJ_DIRECTION::PJ_FWD, coords);

//    std::cout << "CONVERTED : " << converted_coords.xyz/.x << " - " << converted_coords.xyz.y << " - " << converted_coords.xyz.z << std::endl;

    world_x = converted_coords.xyz.x;
    world_y = converted_coords.xyz.y;
    world_z = converted_coords.xyz.z;


}

#endif
