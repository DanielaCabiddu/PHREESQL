#ifndef PHREESQLIB_ENGINE
#define PHREESQLIB_ENGINE

#include <string>

namespace phreesqlib
{

class PhreeSQLibEngine
{
public:
    PhreeSQLibEngine (const std::string db) { db_filename=db; }

    void run_on_folder (const std::string folder);
    void run_on_file (const std::string file);

private:

    std::string db_filename;
};

}

#ifndef PHREESQL_STATIC
#include "PhreeSQLibEngine.cpp"
#endif

#endif
