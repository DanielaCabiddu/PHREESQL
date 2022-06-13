#include "DBEngine.h"
#include "ProJEngine.h"

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

/**
 * @brief phreesqlib::DBEngine::convert_epsg
 * @param epsg
 */
void phreesqlib::DBEngine::convert_epsg (const uint epsg, const std::string filename)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);
    std::vector<std::vector<std::pair<std::string, std::string>>> metadata_table = matrac_reader->getMetadata ();

    phreesqlib::ProjEngine proj_engine;

    ofstream out_file;
    out_file.open(filename.c_str());

    for (uint i=0; i < metadata_table.at(0).size()-1; i++)
        out_file << metadata_table.at(0).at(i).first << ";";

    out_file << metadata_table.at(0).at(metadata_table.at(0).size()-1).first << std::endl;

    for (uint i=0; i < metadata_table.size(); i++)
    {
        double x = DBL_MAX;
        double y = DBL_MAX;
        uint epsg_curr = UINT_MAX;

        for (uint j=0; j < metadata_table.at(i).size(); j++)
        {
            if (metadata_table.at(i).at(j).first.compare("EPSG") == 0)
                epsg_curr = std::atoi(metadata_table.at(i).at(j).second.c_str());
            else if (metadata_table.at(i).at(j).first.compare("COORD_X") == 0)
                x = std::stod(metadata_table.at(i).at(j).second.c_str());
            else if (metadata_table.at(i).at(j).first.compare("COORD_Y") == 0)
                y = std::stod(metadata_table.at(i).at(j).second.c_str());
        }

        if (epsg == epsg_curr) continue;

        double x_converted = DBL_MAX;
        double y_converted = DBL_MAX;
        double z_converted = DBL_MAX;

        proj_engine.epsg2epsg(x, y, 0.0, epsg_curr, epsg, x_converted, y_converted, z_converted);

        for (uint j=0; j < metadata_table.at(i).size(); j++)
        {
            if (metadata_table.at(i).at(j).first.compare("EPSG") == 0)
                out_file << epsg << ";";
            else if (metadata_table.at(i).at(j).first.compare("COORD_X") == 0)
                out_file << std::fixed << x_converted << ";";
            else if (metadata_table.at(i).at(j).first.compare("COORD_Y") == 0)
                out_file << std::fixed << y_converted << ";";
            else
            {
                out_file << metadata_table.at(i).at(j).second ;

                if (j == metadata_table.at(i).size()-1)
                    out_file << std::endl;
                else
                    out_file << ";";
            }
        }
    }

    out_file.close();

    std::cout << "Saved into " << filename << std::endl;

    delete matrac_reader;
}

void phreesqlib::DBEngine::print_DB_summary () const
{
    MatracReader *matrac_reader = new MatracReader(db_filename);

    std::cout << std::endl << " ============== DB SUMMARY ================== " << std::endl << std::endl;

    std::cout << " " << matrac_reader->getMetadataTableName() << " #records : " << matrac_reader->getNumMetadataRecords() << std::endl;

    std::cout << std::endl << " ============================================ " << std::endl << std::endl;


    delete  matrac_reader;
}


void phreesqlib::DBEngine::export_input (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);

    if (analysis_ids.empty())
        matrac_reader->writeAllInputFiles(out_folder, overwrite);
    else
        if (analysis_ids.size()==1)
            matrac_reader->writeInputFile(out_folder, analysis_ids.at(0), overwrite);
    else
            matrac_reader->writeInputFiles(out_folder, analysis_ids, overwrite);

    delete  matrac_reader;
}

void phreesqlib::DBEngine::export_output (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);
    if (analysis_ids.empty())
        matrac_reader->writeAllOutputFiles(out_folder, overwrite);
    else
        if (analysis_ids.size()==1)
            matrac_reader->writeOutputFile(out_folder, analysis_ids.at(0), overwrite);
    else
        matrac_reader->writeOutputFiles(out_folder, analysis_ids, overwrite);
    delete  matrac_reader;
}

void phreesqlib::DBEngine::export_metadata (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);
    if (analysis_ids.empty())
        matrac_reader->writeAllMetadataFiles(out_folder, overwrite);
    else
        if (analysis_ids.size()==1)
            matrac_reader->writeMetadataFile(out_folder, analysis_ids.at(0), overwrite);
    else
        matrac_reader->writeMetadataFiles(out_folder, analysis_ids, overwrite);
    delete  matrac_reader;
}
