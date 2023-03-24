#ifndef DB_ENGINE
#define DB_ENGINE

#include "PhreeqcEngine.h"

#include "matrac-reader.cpp"

namespace phreesqlib
{

///
/// \brief The EPSG_CONVERT_TYPE enum
///
enum EPSG_CONVERT_TYPE
{
    CSV,
    TABLE,
    DB,
    UNSET,
};

///
/// \brief The DBEngine class
///
class DBEngine
{
public:
    ///
    /// \brief DBEngine
    /// \param filename
    ///
    DBEngine (const std::string filename) {
        db_filename = filename;
    }

    ~DBEngine ()
    {
    }

    ///
    /// \brief add_to_DB
    /// \param obj
    /// \param metadata_filename
    ///
    void add_to_DB(const PhreeqcEngineObj &obj, const std::string metadata_filename);

    ///
    /// \brief export_input
    /// \param out_folder
    /// \param analysis_ids
    /// \param overwrite
    ///
    void export_input (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);

    ///
    /// \brief export_output
    /// \param out_folder
    /// \param analysis_ids
    /// \param overwrite
    ///
    void export_output (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);

    ///
    /// \brief export_metadata
    /// \param out_folder
    /// \param analysis_ids
    /// \param overwrite
    ///
    void export_metadata (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite = true);

    ///
    /// \brief convert_epsg
    /// \param epsg
    /// \param types
    /// \param outputs
    ///
    void convert_epsg (const int epsg, const std::vector<EPSG_CONVERT_TYPE> types, std::vector<string> outputs);

    ///
    /// \brief print_DB_summary
    ///
    void print_DB_summary () const;

private:

    ///
    /// \brief db_filename
    ///
    std::string db_filename;

};

}

#ifndef PHREESQL_STATIC
#include "DBEngine.cpp"
#endif

#endif
