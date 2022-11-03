#ifndef DB_ENGINE
#define DB_ENGINE

#include "PhreeqcEngine.h"

#include "matrac-reader.cpp"

namespace phreesqlib
{

enum EPSG_CONVERT_TYPE
{
    CSV,
    TABLE,
    DB,
    UNSET,
};

class DBEngine
{
public:
    DBEngine (const std::string filename) {
        db_filename = filename;
//        matrac_reader = new MatracReader(db_filename);
    }

    ~DBEngine ()
    {
//        delete matrac_reader;
    }

    void add_to_DB(const PhreeqcEngineObj &obj, const std::string metadata_filename);

    void export_input (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);
    void export_output (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);
    void export_metadata (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite = true);

    void convert_epsg (const int epsg, const std::vector<EPSG_CONVERT_TYPE> types, std::vector<string> outputs);

    void print_DB_summary () const;

private:

//    MatracReader *matrac_reader = nullptr;
    std::string db_filename;

};

}

#ifndef PHREESQL_STATIC
#include "DBEngine.cpp"
#endif

#endif
