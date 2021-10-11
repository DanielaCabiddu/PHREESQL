#ifndef PHREEQC_ENGINE
#define PHREEQC_ENGINE

#include "PhreeqcEngineObj.h"
#include <IPhreeqc.hpp>

#include <string>

namespace phreesqlib
{

class PhreeqcEngine
{
public:

    PhreeqcEngine () {}

    std::vector<PhreeqcEngineObj> run_on_folder (const std::string & in_foldername,
                                                 const std::string & out_foldername) ;

    PhreeqcEngineObj run_on_file(const std::string &in_filename, const std::string &out_filename) ;
};

}

#ifndef PHREESQL_STATIC
#include "PhreeqcEngine.cpp"
#endif

#endif
