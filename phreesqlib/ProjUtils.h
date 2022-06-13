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
