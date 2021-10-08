#include "DBEngine.h"

/**
 * @brief phreesqlib::DBEngine::add_to_DB
 * @param obj a phreeqc input/output pair to be added to the DB
 */
void phreesqlib::DBEngine::add_to_DB (const PhreeqcEngineObj &obj, const std::string metadata_filename)
{
    matrac_reader->readInputOutputFiles(obj.get_in_filename(), obj.get_out_filename(), metadata_filename);
}
