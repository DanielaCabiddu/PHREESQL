#ifndef PHREEQCOBJ_ENGINE
#define PHREEQCOBJ_ENGINE

#include <string>

namespace phreesqlib
{

///
/// \brief The PhreeqcEngineObj class
///
class PhreeqcEngineObj
{
public:
    ///
    /// \brief PhreeqcEngineObj
    ///
    PhreeqcEngineObj () {}

    ///
    /// \brief set_error_counter
    /// \param i
    ///
    void set_error_counter  (const unsigned int i)  { error_counter = i; }

    ///
    /// \brief set_in_filename
    /// \param s
    ///
    void set_in_filename    (const std::string s)   { in_filename = s; }

    ///
    /// \brief set_out_filename
    /// \param s
    ///
    void set_out_filename   (const std::string s)   { out_filename = s; }

    ///
    /// \brief get_error_counter
    /// \return
    ///
    unsigned int get_error_counter  () const { return error_counter; }

    ///
    /// \brief get_in_filename
    /// \return
    ///
    std::string  get_in_filename    () const { return in_filename; }

    ///
    /// \brief get_out_filename
    /// \return
    ///
    std::string  get_out_filename   () const { return out_filename; }

private:

    ///
    /// \brief error_counter
    ///
    unsigned int error_counter = 0;

    ///
    /// \brief in_filename
    ///
    std::string in_filename;

    ///
    /// \brief out_filename
    ///
    std::string out_filename;
};


}

//#ifndef PHREESQL_STATIC
//#include "PhreeqcEngine.cpp"
//#endif

#endif
