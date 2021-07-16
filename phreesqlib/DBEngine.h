#ifndef DB_ENGINE
#define DB_ENGINE

#include "PhreeqcEngine.h"

namespace phreesqlib
{

class DBEngine
{
public:
    DBEngine (const std::string filename) { db_filename = filename; }

    void add_to_DB(const PhreeqcEngineObj &obj);

private:

    std::string db_filename;

};

}

#ifndef PHREESQL_STATIC
#include "DBEngine.cpp"
#endif

#endif
