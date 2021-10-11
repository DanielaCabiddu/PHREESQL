#ifndef PHREEQCOBJ_ENGINE
#define PHREEQCOBJ_ENGINE

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


}

//#ifndef PHREESQL_STATIC
//#include "PhreeqcEngine.cpp"
//#endif

#endif
