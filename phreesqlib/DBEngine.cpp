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
void phreesqlib::DBEngine::convert_epsg (const int epsg, const std::vector<EPSG_CONVERT_TYPE> types, std::vector<std::string> outputs)
{
    MatracReader *matrac_reader = new MatracReader(db_filename);
    std::vector<std::vector<std::pair<std::string, std::string>>> metadata_table = matrac_reader->getMetadata ();

    std::vector<std::string> ids;
    std::vector<double> x_vect;
    std::vector<double> y_vect;
    std::vector<double> z_vect;

    phreesqlib::ProjEngine proj_engine;

    for (uint i=0; i < metadata_table.size(); i++)
    {
        double x = DBL_MAX;
        double y = DBL_MAX;
        double z = DBL_MAX;
        uint epsg_curr = UINT_MAX;
        std::string id;

        for (uint j=0; j < metadata_table.at(i).size(); j++)
        {
            try {

            if (metadata_table.at(i).at(j).first.compare("EPSG") == 0)
                epsg_curr = std::atoi(metadata_table.at(i).at(j).second.c_str());
            else if (metadata_table.at(i).at(j).first.compare("ID") == 0)
                id = metadata_table.at(i).at(j).second.c_str();
            else if (metadata_table.at(i).at(j).first.compare("COORD_X") == 0)
                x = std::stod(metadata_table.at(i).at(j).second.c_str());
            else if (metadata_table.at(i).at(j).first.compare("COORD_Y") == 0)
                y = std::stod(metadata_table.at(i).at(j).second.c_str());
            else if (metadata_table.at(i).at(j).first.compare("COORD_Z") == 0)
                z = std::stod(metadata_table.at(i).at(j).second.c_str());


            }  catch (exception e) {
                std::cerr << metadata_table.at(i).at(j).second.c_str() << std::endl;
                std::cerr << e.what() << std::endl;exit(1);
            }
        }

        double x_converted = DBL_MAX;
        double y_converted = DBL_MAX;
        double z_converted = DBL_MAX;

        if (epsg == epsg_curr)
        {
            x_converted = x;
            y_converted = y;
            z_converted = z;
        }
        else
            proj_engine.epsg2epsg(x, y, 0.0, epsg_curr, epsg, x_converted, y_converted, z_converted);

        x_vect.push_back(x_converted);
        y_vect.push_back(y_converted);
        z_vect.push_back(z_converted);

        ids.push_back(id);
    }

    for (uint t=0; t < types.size(); t++)
    {
        if (types.at(t) == TABLE)
        {
            bool success = matrac_reader->create_and_insert_EpsgTable(outputs.at(t), ids, x_vect, y_vect, z_vect);

            if (success)
                std::cout << "Created table " << outputs.at(t) << " in DB : " << db_filename << std::endl;
            else
                std::cerr << "\033[1;31mERROR in creating table " << outputs.at(t) << " in DB : " << db_filename << "\033[0m" << std::endl;
        }

        if (types.at(t) == DB)
        {
            matrac_reader->copyDB(outputs.at(t));

            MatracReader *matrac_reader_copy = new MatracReader(outputs.at(t));

            bool success = matrac_reader_copy->updateAnalysisEPSG (epsg, ids, x_vect, y_vect);

            delete  matrac_reader_copy;

            if (success)
                std::cout << "Created DB : " << outputs.at(t) << std::endl;
            else
                std::cerr << "\033[1;31mERROR in creating DB : " << outputs.at(t) << "\033[0m" << std::endl;
        }

        if (types.at(t) == CSV)
        {
            std::string filename = outputs.at(t);

            ofstream out_file;
            out_file.open(filename.c_str());

            for (uint i=0; i < metadata_table.at(0).size()-1; i++)
                out_file << metadata_table.at(0).at(i).first << ";";

            out_file << metadata_table.at(0).at(metadata_table.at(0).size()-1).first << std::endl;

            for (uint i=0; i < metadata_table.size(); i++)
            {
                for (uint j=0; j < metadata_table.at(i).size(); j++)
                {
                    if (metadata_table.at(i).at(j).first.compare("EPSG") == 0)
                        out_file << epsg << ";";
                    else if (metadata_table.at(i).at(j).first.compare("COORD_X") == 0)
                        out_file << std::fixed << x_vect.at(i) << ";";
                    else if (metadata_table.at(i).at(j).first.compare("COORD_Y") == 0)
                        out_file << std::fixed << y_vect.at(i) << ";";
                    else if (metadata_table.at(i).at(j).first.compare("COORD_Z") == 0)
                        out_file << std::fixed << z_vect.at(i) << ";";
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

            std::cout << "Saved CSV : " << filename << std::endl;
        }
    }

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
