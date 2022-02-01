#include "DBEngine.h"

/**
 * @brief phreesqlib::DBEngine::add_to_DB
 * @param obj a phreeqc input/output pair to be added to the DB
 */
void phreesqlib::DBEngine::add_to_DB (const PhreeqcEngineObj &obj, const std::string metadata_filename)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);
    matrac_reader->readInputOutputFiles(obj.get_in_filename(), obj.get_out_filename(), metadata_filename);
    delete  matrac_reader;
}

void phreesqlib::DBEngine::export_input (const std::string out_folder, const std::vector<int> analysis_ids)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);

    if (analysis_ids.empty())
        matrac_reader->writeAllInputFiles(out_folder);
    else
        if (analysis_ids.size()==1)
            matrac_reader->writeInputFile(out_folder, analysis_ids.at(0));
    else
            matrac_reader->writeInputFiles(out_folder, analysis_ids);

    delete  matrac_reader;
}

void phreesqlib::DBEngine::export_output (const std::string out_folder, const std::vector<int> analysis_ids)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);
    if (analysis_ids.empty())
        matrac_reader->writeAllOutputFiles(out_folder);
    else
        if (analysis_ids.size()==1)
            matrac_reader->writeOutputFile(out_folder, analysis_ids.at(0));
    else
        matrac_reader->writeOutputFiles(out_folder, analysis_ids);
    delete  matrac_reader;
}
