#ifndef PHREESQLIB_ENGINE
#define PHREESQLIB_ENGINE

#include "DBEngine.h"
#include <string>
#include <vector>

namespace phreesqlib
{

class PhreeSQLibEngine
{
public:
    PhreeSQLibEngine (const std::string db) { db_filename=db; }

    void run_on_folder (const std::string in_folder, const std::string out_folder, const std::string meta_folder);

    void run_phreeqc_on_folder (const std::string in_folder, const std::string out_folder, const std::string phreeqc_db_path);

    void export_input (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);
    void export_output (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);
    void export_metadata (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite = true);

    void epsg_convert (const int epsg, const std::vector<EPSG_CONVERT_TYPE> types, std::vector<std::string> outputs);

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
