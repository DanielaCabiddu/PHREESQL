#include "PhreeSQLibEngine.h"

#include "PhreeqcEngine.h"
#include "DBEngine.h"

/**
 * @brief phreesqlib::PhreeSQLibEngine::run_on_folder
 * @param folder
 */
void phreesqlib::PhreeSQLibEngine::run_on_folder (const std::string folder)
{
    std::vector<phreesqlib::PhreeqcEngineObj> results;

    phreesqlib::PhreeqcEngine *pqc_engine = new phreesqlib::PhreeqcEngine ();
    results = pqc_engine->run_on_folder(folder);

    delete pqc_engine;

    phreesqlib::DBEngine *db_engine = new phreesqlib::DBEngine (db_filename);

    for (const phreesqlib::PhreeqcEngineObj & obj : results)
    {
        if (obj.get_error_counter() > 0)
        {
            std::cerr << "Some error occured while processing " << obj.get_in_filename()
                      << ". IGNORED. " << std::endl;
            continue;
        }

        db_engine->add_to_DB(obj);
    }

    delete db_engine;
}

/**
 * @brief phreesqlib::PhreeSQLibEngine::run_on_file
 * @param file
 */
void phreesqlib::PhreeSQLibEngine::run_on_file (const std::string file)
{
    phreesqlib::PhreeqcEngine *pqc_engine = new phreesqlib::PhreeqcEngine ();
    PhreeqcEngineObj obj = pqc_engine->run_on_file(file, file + ".pqo");

    delete pqc_engine;

    if (obj.get_error_counter() > 0)
    {
        std::cerr << "Error while processing " << file << std::endl;
        return;
    }

    phreesqlib::DBEngine *db_engine = new phreesqlib::DBEngine (db_filename);
    db_engine->add_to_DB(obj);

    delete pqc_engine;
}
