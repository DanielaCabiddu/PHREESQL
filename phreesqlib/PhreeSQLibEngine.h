#ifndef PHREESQLIB_ENGINE
#define PHREESQLIB_ENGINE

#include <string>

namespace phreesqlib
{

class PhreeSQLibEngine
{
public:
    PhreeSQLibEngine (const std::string db) { db_filename=db; }

    void run_on_folder (const std::string in_folder, const std::string out_folder, const std::string meta_folder);

private:

    std::string db_filename;

    const std::string in_ext = ".pqi";
    const std::string out_ext = ".pqo";
    const std::string meta_ext = ".met";
};

}

#ifndef PHREESQL_STATIC
#include "PhreeSQLibEngine.cpp"
#endif

#endif
