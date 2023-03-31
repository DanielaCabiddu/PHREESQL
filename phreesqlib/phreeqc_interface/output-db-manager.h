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

#ifndef PHREESQLIB_OUTPUT_DB_MANAGER
#define PHREESQLIB_OUTPUT_DB_MANAGER

#include "db-manager.h"

///
/// \brief The OutputDBManager class
///
class OutputDBManager : public DBManager
{
private:
    ///
    /// \brief a_id
    ///
    int a_id;

public:
    ///
    /// \brief OutputDBManager
    /// \param db
    ///
    OutputDBManager(sqlite3 *db) : DBManager(db) {}

    ~OutputDBManager () {}

    ///
    /// \brief isOutputEmpty
    /// \return
    ///
    bool isOutputEmpty();

    ///
    /// \brief createEpsgTable
    /// \param table_name
    /// \return
    ///
    bool createEpsgTable(const std::string table_name);

    ///
    /// \brief createAnalisysTable
    ///
    void createAnalisysTable();

    ///
    /// \brief createSolutionCompositionTable
    ///
    void createSolutionCompositionTable();

    ///
    /// \brief createDescriptionOfSolutionTable
    ///
    void createDescriptionOfSolutionTable();

    ///
    /// \brief createDistributionOfSpeciesTable
    ///
    void createDistributionOfSpeciesTable();

    ///
    /// \brief createDistributionOfAlkalinityTable
    ///
    void createDistributionOfAlkalinityTable();

    ///
    /// \brief createSaturationIndicesTable
    ///
    void createSaturationIndicesTable();

    ///
    /// \brief insertEpsg
    /// \param table_name
    /// \param id
    /// \param x
    /// \param y
    /// \param z
    /// \return
    ///
    bool insertEpsg(const std::string table_name,
                   const std::vector<std::string> &id,
                   const std::vector<double> &x,
                   const std::vector<double> &y,
                   const std::vector<double> &z);

    ///
    /// \brief insertAnalisys
    /// \param a
    /// \param meta
    /// \return
    ///
    int insertAnalisys(Analisys &a, metadata meta);

    ///
    /// \brief insertSolutionComposition
    /// \param sc_list
    ///
    void insertSolutionComposition(vector<SolutionComposition> sc_list);

    ///
    /// \brief insertDescriptionOfSolution
    /// \param des_list
    ///
    void insertDescriptionOfSolution(vector<DescriptionOfSolution> des_list);

    ///
    /// \brief insertDistributionOfSpecies
    /// \param dis_list
    ///
    void insertDistributionOfSpecies(vector<DistributionOfSpecies> dis_list);

    ///
    /// \brief insertDistributionOfAlkalinity
    /// \param alk_list
    ///
    void insertDistributionOfAlkalinity(vector<DistributionOfAlkalinity> alk_list);

    ///
    /// \brief insertSaturationIndices
    /// \param si_list
    ///
    void insertSaturationIndices(vector<SaturationIndices> si_list);

    ///
    /// \brief selectValuesfromAnalisys
    /// \param a
    /// \param analysis_id
    /// \return
    ///
    bool selectValuesfromAnalisys(Analisys &a, int analysis_id);

    ///
    /// \brief selectValuesFromSolutionComposition
    /// \param sc_list
    /// \param analisys_num
    ///
    void selectValuesFromSolutionComposition(vector<SolutionComposition> &sc_list, int analisys_num);

    ///
    /// \brief selectValuesFromDescriptionOfSolution
    /// \param des_list
    /// \param analisys_num
    ///
    void selectValuesFromDescriptionOfSolution(vector<DescriptionOfSolution> &des_list, int analisys_num);

    ///
    /// \brief selectValuesFromDistributionOfSpecies
    /// \param dis_list
    /// \param analisys_num
    ///
    void selectValuesFromDistributionOfSpecies(vector<DistributionOfSpecies> &dis_list, int analisys_num);

    ///
    /// \brief selectValuesFromDistributionOfAlkalinity
    /// \param alk_list
    /// \param analisys_num
    ///
    void selectValuesFromDistributionOfAlkalinity(vector<DistributionOfAlkalinity> &alk_list, int analisys_num);

    ///
    /// \brief selectValuesFromSaturationIndices
    /// \param si_list
    /// \param analisys_num
    ///
    void selectValuesFromSaturationIndices(vector<SaturationIndices> &si_list, int analisys_num);
};

#ifndef PHREESQL_STATIC
#include "output-db-manager.cpp"
#endif

#endif
