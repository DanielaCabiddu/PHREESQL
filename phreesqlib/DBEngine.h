#ifndef DB_ENGINE
#define DB_ENGINE

#include "PhreeqcEngine.h"

#include "matrac-reader.cpp"

namespace phreesqlib
{

class DBEngine
{
public:
    DBEngine (const std::string filename) {
        db_filename = filename;
        matrac_reader = new MatracReader(db_filename);
    }

    ~DBEngine ()
    {
        delete matrac_reader;
    }

    void add_to_DB(const PhreeqcEngineObj &obj, const std::string metadata_filename);

private:

    MatracReader *matrac_reader = nullptr;
    std::string db_filename;

};

}

#ifndef PHREESQL_STATIC
#include "DBEngine.cpp"
#endif

#endif
