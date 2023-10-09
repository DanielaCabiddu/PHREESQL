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

#ifndef PHREESQLIB_OUTPUT_READER_WRITER
#define PHREESQLIB_OUTPUT_READER_WRITER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "utils.h"
#include "models.h"
using namespace std;

///
/// \brief The OutputReaderWriter class
///
class OutputReaderWriter
{

public:
    ///
    /// \brief sc_list
    ///
    vector<SolutionComposition> sc_list;

    ///
    /// \brief des_list
    ///
    vector<DescriptionOfSolution> des_list;

    ///
    /// \brief dis_list
    ///
    vector<DistributionOfSpecies> dis_list;

    ///
    /// \brief si_list
    ///
    vector<SaturationIndices> si_list;

    ///
    /// \brief alk_list
    ///
    vector<DistributionOfAlkalinity> alk_list;

    ///
    /// \brief a
    ///
    Analisys a;

    ///
    /// \brief OutputReaderWriter
    ///
    OutputReaderWriter() {}

    ///
    /// \brief checkSection
    /// \param size
    /// \param section
    /// \param file
    ///
    void checkSection(int size, string section, string file);

    ///
    /// \brief readMetadata
    /// \param path
    /// \return
    ///
    bool readMetadata(string path);

    ///
    /// \brief insertInAnalisys
    /// \param key
    /// \param value
    ///
    void insertInAnalisys(string key, string value);

    ///
    /// \brief readFile
    /// \param path
    /// \return
    ///
    bool readFile(string path);

    ///
    /// \brief readSolutionComposition
    /// \param file
    /// \param row
    ///
    void readSolutionComposition(ifstream &file, int &row);

    ///
    /// \brief readDescriptionOfSolution
    /// \param file
    /// \param row
    ///
    void readDescriptionOfSolution(ifstream &file, int &row);

    ///
    /// \brief readDistributionOfSpecies
    /// \param file
    /// \param row
    ///
    void readDistributionOfSpecies(ifstream &file, int &row);

    ///
    /// \brief readDistributionOfAlkalinity
    /// \param file
    /// \param row
    ///
    void readDistributionOfAlkalinity(ifstream &file, int &row);

    ///
    /// \brief readSaturationIndices
    /// \param file
    /// \param row
    ///
    void readSaturationIndices(ifstream &file, int &row);

    ///
    /// \brief writeAnalisysFile
    /// \param path
    /// \param analisys_id
    /// \param input_list
    /// \return
    ///
    bool writeAnalisysFile(string path, int analisys_id, const vector<SolutionInput> &input_list);

    ///
    /// \brief writeMetadata
    /// \param file_path
    ///
    void writeMetadata(const std::string file_path);

    ///
    /// \brief writeSolutionComposition
    /// \param file
    ///
    void writeSolutionComposition(fstream &file);

    ///
    /// \brief writeDescriptionOfSolution
    /// \param file
    ///
    void writeDescriptionOfSolution(fstream &file);

    ///
    /// \brief writeDistributionOfSpecies
    /// \param file
    ///
    void writeDistributionOfSpecies(fstream &file);

    ///
    /// \brief writeDistributionOfAlkalinity
    /// \param file
    ///
    void writeDistributionOfAlkalinity(fstream &file);

    ///
    /// \brief writeSaturationIndices
    /// \param file
    ///
    void writeSaturationIndices(fstream &file);
};

#ifndef PHREESQL_STATIC
#include "output-reader-writer.cpp"
#endif

#endif

