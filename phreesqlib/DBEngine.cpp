#include "DBEngine.h"

#include "matrac-reader.cpp"

/**
 * @brief phreesqlib::DBEngine::add_to_DB
 * @param obj a phreeqc input/output pair to be added to the DB
 */
void phreesqlib::DBEngine::add_to_DB (const PhreeqcEngineObj &obj)
{
    MatracReader *matrac = new MatracReader(db_filename);
    matrac->readInputOutputFiles(obj.get_in_filename(), obj.get_out_filename(), "");
}
