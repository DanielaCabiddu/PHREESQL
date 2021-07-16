#ifndef PHREEQC_ENGINE
#define PHREEQC_ENGINE

#include <IPhreeqc.hpp>

#include <string>

namespace phreesqlib
{

class PhreeqcEngineObj
{
public:
    PhreeqcEngineObj () {}

    void set_error_counter  (const unsigned int i)  { error_counter = i; }
    void set_in_filename    (const std::string s)   { in_filename = s; }
    void set_out_filename   (const std::string s)   { out_filename = s; }

    unsigned int get_error_counter  () const { return error_counter; }
    std::string  get_in_filename    () const { return in_filename; }
    std::string  get_out_filename   () const { return out_filename; }

private:

    unsigned int error_counter = 0;
    std::string in_filename;
    std::string out_filename;
};

class PhreeqcEngine
{
public:

    PhreeqcEngine () {}

    std::vector<PhreeqcEngineObj> run_on_folder (const std::string &foldername) ;
    PhreeqcEngineObj run_on_file(const std::string &in_filename, const std::string &out_filename) ;
};

}

#ifndef PHREESQL_STATIC
#include "PhreeqcEngine.cpp"
#endif

#endif
