#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

#ifndef MODELS
#define MODELS
#pragma once

typedef map<string, string> metadata;

struct SolutionInput
{
    int analisys_id;
    string solution_number;
    string unit;
    string ppm;
};

struct SolutionComposition
{
    int analisys_id;
    string element;
    string molality;
    string moles;
};

struct DescriptionOfSolution
{
    int analisys_id;
    string key_name;
    string value;
};

struct DistributionOfSpecies
{
    int analisys_id;
    string species;
    string molality;
    string activity;
    string log_molality;
    string log_activity;
    string log_gamma;
};

struct SaturationIndices
{
    int analisys_id;
    string phase;
    string si;
    string log_iap;
    string log_k;
    string formula;
};

struct DistributionOfAlkalinity
{
    int analisys_id;
    string species;
    string alkalinity;
    string molality;
    string alk_mol;
};

struct Analisys
{
    int id;
    string job_type;
    string survey;
    string site_name;
    string date;
    string database;
    string phreeqc_version;
    string run_number;
    string sample_name;
    string input_file;
    string coord_x;
    string coord_y;
    string epsg;
    string timestamp;
};

inline
void printSolutionInputs(vector<SolutionInput> input_list)
{
    for (int i = 0; i < input_list.size(); i++)
    {
        cout << input_list[i].unit << " " << input_list[i].ppm << endl;
    }
}

inline
void printMetadata(metadata meta)
{
    for (map<string, string>::const_iterator it = meta.begin(); it != meta.end(); ++it)
    {
        cout << it->first << " " << it->second << endl;
    }
}

inline
void printSolutionComposition(vector<SolutionComposition> sc_list)
{
    for (int i = 0; i < sc_list.size(); i++)
    {
        cout << sc_list[i].element << " " << sc_list[i].molality << " " << sc_list[i].moles << endl;
    }
}

inline
void printDescriptionOfSolution(vector<DescriptionOfSolution> dos_list)
{
    for (int i = 0; i < dos_list.size(); i++)
    {
        cout << dos_list[i].key_name << " " << dos_list[i].value << endl;
    }
}

inline
void printDistributionOfSpecies(vector<DistributionOfSpecies> dis_list)
{
    for (int i = 0; i < dis_list.size(); i++)
    {
        cout << dis_list[i].species << " "
             << dis_list[i].molality << " "
             << dis_list[i].activity << " "
             << dis_list[i].log_molality << " "
             << dis_list[i].log_activity << " "
             << dis_list[i].log_gamma << endl;
    }
}

inline
void printDistributionOfAlkalinity(vector<DistributionOfAlkalinity> alk_list)
{
    for (int i = 0; i < alk_list.size(); i++)
    {
        cout << alk_list[i].species << " "
             << alk_list[i].alkalinity << " "
             << alk_list[i].molality << " "
             << alk_list[i].alk_mol << endl;
    }
}

inline
void printSaturationIndices(vector<SaturationIndices> is_list)
{
    for (int i = 0; i < is_list.size(); i++)
    {
        cout << is_list[i].phase << " "
             << is_list[i].si << " "
             << is_list[i].log_iap << " "
             << is_list[i].log_k << " "
             << is_list[i].formula << endl;
    }
}

#endif
