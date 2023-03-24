#ifndef PHREEQC_ENGINE
#define PHREEQC_ENGINE

#include "PhreeqcEngineObj.h"
#include <IPhreeqc.hpp>

#include <string>

namespace phreesqlib
{
///
/// \brief The PhreeqcEngine class
///
class PhreeqcEngine
{
public:

    ///
    /// \brief PhreeqcEngine
    ///
    PhreeqcEngine () {}

    ///
    /// \brief run_on_folder
    /// \param in_foldername
    /// \param out_foldername
    /// \return
    ///
    std::vector<PhreeqcEngineObj> run_on_folder (const std::string & in_foldername,
                                                 const std::string & out_foldername) ;

    ///
    /// \brief run_on_file
    /// \param in_filename
    /// \param out_filename
    /// \return
    ///
    PhreeqcEngineObj run_on_file(const std::string &in_filename, const std::string &out_filename) ;
};

}

#ifndef PHREESQL_STATIC
#include "PhreeqcEngine.cpp"
#endif

#endif
