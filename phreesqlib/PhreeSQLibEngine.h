/********************************************************************************
*  This file is part of PhreeSQL                                                *
*  Copyright(C) 2023: Daniela Cabiddu                                           *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Daniela Cabiddu (daniela.cabiddu@cnr.it)                                  *
*                                                                               *
*     Italian National Research Council (CNR)                                   *
*     Institute for Applied Mathematics and Information Technologies (IMATI)    *
*     Via de Marini, 6                                                          *
*     16149 Genoa,                                                              *
*     Italy                                                                     *
*                                                                               *
*  This program is free software: you can redistribute it and/or modify it      *
*  under the terms of the GNU General Public License as published by the        *
*  Free Software Foundation, either version 3 of the License, or (at your       *
*  option) any later version.                                                   *
*                                                                               *
*  This program is distributed in the hope that it will be useful, but          *
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY   *
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
*  more details.                                                                *
*                                                                               *
*  You should have received a copy of the GNU General Public License along      *
*  with this program. If not, see <https://www.gnu.org/licenses/>.              *
*                                                                               *
*********************************************************************************/

#ifndef PHREESQLIB_ENGINE
#define PHREESQLIB_ENGINE

#include "DBEngine.h"
#include <string>
#include <vector>

/*! \file
 *  \brief PhreeSQLib Engine
 */
namespace phreesqlib
{

///
/// \brief The PhreeSQLibEngine class
///
/// \details The PhreeSQLibEngine class is the PHREESQL engine.
/// It exhibits all the PHREESQL capabilities, being able to run PHREEQC speciation calculation,
/// build a SQL database from scratch and to update an existing database with additional data,
/// export both original PHREEQC inputs and outputs from an existing database for further processing, and
/// process coordinates of geo-referenced data into an existing database to create a new table or
/// to create a new database framed into a new specific coordinate reference system (CRS).
///
class PhreeSQLibEngine
{
public:
    ///
    /// \brief PhreeSQLibEngine creates a connection with a SQL database (stores the path of the database to be further created or updated).
    /// \param db   the path of the PHREESQL database to be either created or updated.
    ///
    PhreeSQLibEngine (const std::string db) { db_filename=db; }

    ///
    /// \brief run_on_folder runs PHREESQL for a collection of analysis.
    /// It assumes that both PHREEQC inputs and outputs are available, together with the corresponding metadata files.
    ///
    /// \param in_folder    the path of the folder where the inputs for PHREEQC are located.
    /// \param out_folder   the path of the folder where the outputs of PHREEQC are located.
    /// \param meta_folder  the path of the folder where the metadata files are located.
    ///
    void run_on_folder (const std::string in_folder, const std::string out_folder, const std::string meta_folder);

    ///
    /// \brief run_phreeqc_on_folder runs PHREEQC speciation calculation for a collection of analysis.
    ///
    /// \param in_folder        the path of the folder where the inputs for PHREEQC are located.
    /// \param out_folder       the path of the folder where the outputs of PHREEQC will be stored.
    /// \param phreeqc_db_path  the path of the PHREEQC thermodynamic database to be used for the speciation calculation.
    ///
    void run_phreeqc_on_folder (const std::string in_folder, const std::string out_folder, const std::string phreeqc_db_path);

    ///
    /// \brief export_input queries PHREESQL database for PHREEQC inputs and stores them on files according to PHREEQC input data format.
    /// It provides the possibility to save the inputs of all the available analysis or of a subset of them.
    ///
    /// \param out_folder       the path of the folder where the inputs for PHREEQC will be stored.
    /// \param analysis_ids     the list of IDS of the subset of analyses of interest (optional)
    /// \param overwrite        specifies if existing files can be overwritten. Default: TRUE. (optional)
    ///
    void export_input (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);

    ///
    /// \brief export_output queries PHREESQL database for PHREEQC outputs and stores them on files according to PHREEQC output data format.
    /// It provides the possibility to save the outputs of all the available analysis or of a subset of them.
    ///
    /// \param out_folder       the path of the folder where the outputs of PHREEQC will be stored.
    /// \param analysis_ids     the list of IDS of the subset of analyses of interest (optional)
    /// \param overwrite        specifies if existing files can be overwritten. Default: TRUE. (optional)
    ///
    void export_output (const std::string out_folder, const std::vector<int> analysis_ids = std::vector<int> (), const bool overwrite = true);

    ///
    /// \brief export_metadata  queries PHREESQL database analysis metadata and stores them on files according to PHREESQL matadata file format.
    /// It provides the possibility to save metadata of all the available analysis or of a subset of them.
    ///
    /// \param out_folder       the path of the folder where metadata file will be stored.
    /// \param analysis_ids     the list of IDS of the subset of analyses of interest (optional)
    /// \param overwrite        specifies if existing files can be overwritten. Default: TRUE. (optional)
    ///
    void export_metadata (const std::string out_folder, const std::vector<int> analysis_ids, const bool overwrite = true);

    ///
    /// \brief epsg_convert quesries PHREESQL database and, for each coordinate, performs an EPSG conversion from the input EPSG to a given one.
    ///
    /// \param epsg     output EPSG
    /// \param types    specifies how to save the result (in a csv file, in a new database table, or in a new database) - multiple outputs are supported
    /// \param outputs  for each output type, it specifies the name of the output (the name of the csv file, of the new database table or of the new database).
    ///
    void epsg_convert (const int epsg, const std::vector<DBEngine::EPSG_CONVERT_TYPE> types, std::vector<std::string> outputs);

private:

    ///
    /// \brief db_filename  The PHREESQL database to be either created or updated.
    ///
    std::string db_filename;

    ///
    /// \brief in_ext       PHREEQC input file extension.
    ///
    const std::string in_ext = ".pqi";

    ///
    /// \brief out_ext      PHREEQC ouput file extension.
    ///
    const std::string out_ext = ".pqo";

    ///
    ////// \brief meta_ext  PHREESQL metadata file extension.
    ///
    const std::string meta_ext = ".met";
};

}

#ifndef PHREESQL_STATIC
#include "PhreeSQLibEngine.cpp"
#endif

#endif
