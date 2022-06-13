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
