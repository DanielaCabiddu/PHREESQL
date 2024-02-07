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

#ifndef PHREESQLIB_MATRAC_READER
#define PHREESQLIB_MATRAC_READER

#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "utils.h"
#include "models.h"
#include "input-db-manager.h"
#include "output-db-manager.h"
#include "data-manager.h"
#include "input-reader-writer.h"
#include "output-reader-writer.h"

///
/// \brief The PhreeqcInterface class
///
class PhreeqcInterface
{
private:
    ///
    /// \brief i_manager
    ///
    InputDBManager *i_manager;

    ///
    /// \brief o_manager
    ///
    OutputDBManager *o_manager;

    ///
    /// \brief d_manager
    ///
    DataManager *d_manager;

    ///
    /// \brief i_file
    ///
    InputReaderWriter i_file;

    ///
    /// \brief o_file
    ///
    OutputReaderWriter o_file;

    ///
    /// \brief db_path
    ///
    string db_path;

    ///
    /// \brief db
    ///
    sqlite3 *db;

    ///
    /// \brief rc
    ///
    int rc;

    ///
    /// \brief is_i_file
    ///
    bool is_i_file;

    ///
    /// \brief is_o_file
    ///
    bool is_o_file;

    ///
    /// \brief openDB
    /// \return
    ///
    bool openDB();

    ///
    /// \brief closeDB
    ///
    void closeDB();

    ///
    /// \brief deleteFileIfExists
    /// \param path
    ///
    void deleteFileIfExists(string path);

    ///
    /// \brief openDir
    /// \param directory
    /// \return
    ///
    DIR *openDir(string directory);

public:
    ///
    /// \brief MatracReader
    /// \param db_path
    ///
    PhreeqcInterface(const std::string db_path);

    ~PhreeqcInterface()
    {
        delete i_manager;
        delete o_manager;
        delete d_manager;

        sqlite3_close(db);
    }

    ///
    /// \brief copyDB
    /// \param file_path
    ///
    void copyDB (const std::string file_path);

    ///
    /// \brief getMetadata
    /// \return
    ///
    std::vector<std::vector<std::pair<std::string, std::string>>> getMetadata () const;

    ///
    /// \brief readInputOutputFiles
    /// \param input_path
    /// \param output_path
    /// \param meta_path
    ///
    void readInputOutputFiles(const string input_path, const string output_path, const string meta_path);

    ///
    /// \brief readIOMFiles
    /// \param ifile_path
    /// \param ofile_path
    /// \param meta_path
    /// \return
    ///
    bool readIOMFiles(const string ifile_path, const string ofile_path, const string meta_path);

    ///
    /// \brief writeInputFile
    /// \param directory
    /// \param analysis_id
    /// \param overwrite
    ///
    void writeInputFile(const string directory, int analysis_id, const bool overwrite);

    ///
    /// \brief writeInputFiles
    /// \param directory
    /// \param analysis_ids
    /// \param overwrite
    ///
    void writeInputFiles(const string directory, const std::vector<int> &analysis_ids, const bool overwrite);

    ///
    /// \brief writeAllInputFiles
    /// \param directory
    /// \param overwrite
    ///
    void writeAllInputFiles(const string directory, const bool overwrite);

    //////////////////////////////////////////////////////////////////////////////////////

    ///
    /// \brief writeMetadataFile
    /// \param directory
    /// \param analysis_id
    /// \param overwrite
    ///
    void writeMetadataFile(const string directory, int analysis_id, const bool overwrite);

    ///
    /// \brief writeMetadataFiles
    /// \param directory
    /// \param analysis_ids
    /// \param overwrite
    ///
    void writeMetadataFiles(const string directory, const std::vector<int> &analysis_ids, const bool overwrite);

    ///
    /// \brief writeAllMetadataFiles
    /// \param directory
    /// \param overwrite
    ///
    void writeAllMetadataFiles(const string directory, const bool overwrite);

    //////////////////////////////////////////////////////////////////////////////////////

    ///
    /// \brief writeOutputFile
    /// \param directory
    /// \param analysis_id
    /// \param overwrite
    ///
    void writeOutputFile(const string directory, int analysis_id, const bool overwrite);

    ///
    /// \brief writeOutputFiles
    /// \param directory
    /// \param analysis_ids
    /// \param overwrite
    ///
    void writeOutputFiles(const string directory, const std::vector<int> &analysis_ids, const bool overwrite);

    ///
    /// \brief writeAllOutputFiles
    /// \param directory
    /// \param overwrite
    ///
    void writeAllOutputFiles(const string directory, const bool overwrite);

    ///
    /// \brief createInputTables
    ///
    void createInputTables();

    ///
    /// \brief createOutputTables
    ///
    void createOutputTables();

    ///
    /// \brief create_and_insert_EpsgTable
    /// \param table_name
    /// \param id
    /// \param x
    /// \param y
    /// \param z
    /// \return
    ///
    bool create_and_insert_EpsgTable(const std::string table_name,
                                     const std::vector<std::string> &id,
                                     const std::vector<double> &x,
                                     const std::vector<double> &y,
                                     const std::vector<double> &z);

    ///
    /// \brief updateAnalysisEPSG
    /// \param epsg
    /// \param id
    /// \param x
    /// \param y
    /// \return
    ///
    bool updateAnalysisEPSG (const unsigned int epsg,
                             const std::vector<std::string> &id,
                             const std::vector<double> &x,
                             const std::vector<double> &y);

    ///
    /// \brief getData
    /// \param table_name
    /// \param value
    /// \param column_name
    /// \param timestamp
    /// \return
    ///
    vector<vector<string> > getData(string table_name, string value, string column_name, string timestamp);

    ///
    /// \brief writeData
    /// \param file_path
    /// \param data
    ///
    void writeData(string file_path, vector<vector<string> > data);

    ///
    /// \brief getMetadataTableName
    /// \return
    ///
    std::string getMetadataTableName () const;

    ///
    /// \brief getNumMetadataRecords
    /// \return
    ///
    unsigned int getNumMetadataRecords () const;
};

#ifndef PHREESQL_STATIC
#include "matrac-reader.cpp"
#endif

#endif

