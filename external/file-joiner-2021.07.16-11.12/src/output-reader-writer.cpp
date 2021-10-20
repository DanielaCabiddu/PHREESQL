#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "utils.h"
#include "models.h"
using namespace std;

class OutputReaderWriter
{

public:
    vector<SolutionComposition> sc_list;
    vector<DescriptionOfSolution> des_list;
    vector<DistributionOfSpecies> dis_list;
    vector<SaturationIndices> si_list;
    vector<DistributionOfAlkalinity> alk_list;
    Analisys a;

    OutputReaderWriter() {}

    void checkSection(int size, string section, string file)
    {
        if (size == 0)
        {
            cerr << "Error in reading section '" << section << "' in file '" << file << "'" << endl;
        }
    }

    bool readMetadata(string path)
    {
        ifstream input_file;
        string line, key, value;
        input_file.open(path, ios::in); //open a file to perform read operation using file object

        if (input_file.is_open())
        {
            while (getline(input_file, line))
            {

                if (line.size() > 1)
                {
                    key = trim(line.substr(0, line.find(":")));
                    value = trim(line.substr(line.find(":") + 1));
                    insertInAnalisys(key, value);
                }
            }
            return true;
        }
        return false;
    }

    void insertInAnalisys(string key, string value)
    {

        if (strcmp(key.c_str(), "JOB TYPE") == 0)
        {
            a.job_type = value;
        }
        else if (strcmp(key.c_str(), "SURVEY") == 0)
        {
            a.survey = value;
        }
        else if (strcmp(key.c_str(), "DATE") == 0)
        {
            a.date = value;
        }
        else if (strcmp(key.c_str(), "DATABASE") == 0)
        {
            a.database = value;
        }
        else if (strcmp(key.c_str(), "PHREEQC_VERSION") == 0)
        {
            a.phreeqc_version = value;
        }
        else if (strcmp(key.c_str(), "RUN_NUMBER") == 0)
        {
            a.run_number = value;
        }
        else if (strcmp(key.c_str(), "SAMPLE_NAME") == 0)
        {
            a.sample_name = value;
        }
        else if (strcmp(key.c_str(), "SITE") == 0)
        {
            a.site_name = value;
        }
        else if (strcmp(key.c_str(), "INPUT_FILE") == 0)
        {
            a.input_file = value;
        }
        else if (strcmp(key.c_str(), "COORD_X") == 0)
        {
            a.coord_x = value;
        }
        else if (strcmp(key.c_str(), "COORD_Y") == 0)
        {
            a.coord_y = value;
        }
        else if (strcmp(key.c_str(), "EPSG") == 0)
        {
            a.epsg = value;
        }
        else if (strcmp(key.c_str(), "TIMESTAMP") == 0)
        {
            a.timestamp = value;
        }
        else
        {
            cerr << "Key " << key << " not found!" << endl;
        }
    }

    bool readFile(string path)
    {
        ifstream input_file;
        int i = 0;

        input_file.open(path, ios::in); //open a file to perform read operation using file object
        if (input_file.is_open())
        { //checking whether the file is open
            string line;

            while (getline(input_file, line))
            { //read data from file object and put it into string.
                i++;
                if (line.rfind('-', 0) == 0)
                {
                    if (strstr(line.c_str(), "Solution composition") != NULL)
                    {
                        readSolutionComposition(input_file, i);
                    }
                    else if (strstr(line.c_str(), "Description of solution") != NULL)
                    {
                        readDescriptionOfSolution(input_file, i);
                    }
                    else if (strstr(line.c_str(), "Distribution of species") != NULL)
                    {
                        readDistributionOfSpecies(input_file, i);
                    }
                    else if (strstr(line.c_str(), "Distribution of alkalinity") != NULL)
                    {
                        readDistributionOfAlkalinity(input_file, i);
                    }
                    else if (strstr(line.c_str(), "Saturation indices") != NULL)
                    {
                        readSaturationIndices(input_file, i);
                    }
                }
            }
            input_file.close(); //close the file object.

            checkSection(sc_list.size(), "Solution composition", path);
            checkSection(des_list.size(), "Description of solution", path);
            checkSection(dis_list.size(), "Distribution of species", path);
            checkSection(dis_list.size(), "Distribution of alkalinity", path);
            checkSection(si_list.size(), "saturation indices", path);

            return true;
        }
        return false;
    }

    void readSolutionComposition(ifstream &file, int &row)
    {
        string line, first, second, third;

        removeHeaders1(file, "Elements", row);

        while (getline(file, line) && line.size() > 1)
        {
            row++;
            istringstream iss(line);
            SolutionComposition sc;
            if (!(iss >> first >> second >> third))
            {
                break;
            }

            sc.element = first;
            sc.molality = second;
            sc.moles = third;
            this->sc_list.push_back(sc);
        }
        if (line.size() == 0)
        {
            row++;
        }
        //printSolutionComposition(this->sc_list);
    }

    void readDescriptionOfSolution(ifstream &file, int &row)
    {
        string line, key, value;

        removeHeaders1(file, "", row);

        while (getline(file, line) && line.size() > 1)
        {
            row++;
            istringstream iss(line);
            DescriptionOfSolution des;
            key = trim(line.substr(0, line.find('=')));
            value = trim(line.substr(line.find('=') + 1, line.size()));
            des.key_name = key;
            des.value = value;
            this->des_list.push_back(des);
        }
        if (line.size() == 0)
        {
            row++;
        }
        //printDescriptionOfSolution(this->des_list);
    }

    void readDistributionOfSpecies(ifstream &file, int &row)
    {
        string line, value;

        removeHeaders1(file, "Species", row);

        while (getline(file, line) && line.size() > 1)
        {
            row++;
            istringstream iss(line);
            DistributionOfSpecies dis;

            for (int i = 0; i < 6; i++)
            {
                if (iss >> value)
                {
                    switch (i)
                    {
                    case 0:
                        dis.species = value;
                        break;
                    case 1:
                        dis.molality = value;
                        break;
                    case 2:
                        dis.activity = value;
                        break;
                    case 3:
                        dis.log_molality = value;
                        break;
                    case 4:
                        dis.log_activity = value;
                        break;
                    case 5:
                        dis.log_gamma = value;
                        break;
                    }
                }
            }
            this->dis_list.push_back(dis);
        }
        if (line.size() == 0)
        {
            row++;
        }
        //printDistributionOfSpecies(this->dis_list);
    }

    void readDistributionOfAlkalinity(ifstream &file, int &row)
    {
        string line, value;

        removeHeaders1(file, "Species", row);

        while (getline(file, line) && line.size() > 1)
        {
            row++;
            istringstream iss(line);
            DistributionOfAlkalinity alk;

            for (int i = 0; i < 4; i++)
            {
                if (iss >> value)
                {
                    switch (i)
                    {
                    case 0:
                        alk.species = value;
                        break;
                    case 1:
                        alk.alkalinity = value;
                        break;
                    case 2:
                        alk.molality = value;
                        break;
                    case 3:
                        alk.alk_mol = value;
                        break;
                    }
                }
            }

            this->alk_list.push_back(alk);
        }
        if (line.size() == 0)
        {
            row++;
        }
        //printDistributionOfAlkalinity(this->alk_list);
    }

    void readSaturationIndices(ifstream &file, int &row)
    {
        string line, phase, si, log_iap, log_k, formula;

        removeHeaders1(file, "Phase", row);

        while (getline(file, line) && line.size() > 1)
        {
            row++;
            istringstream iss(line);
            SaturationIndices sin;

            if (!(iss >> phase >> si >> log_iap >> log_k >> formula))
            {
                break;
            }

            sin.phase = phase;
            sin.si = si;
            sin.log_iap = log_iap;
            sin.log_k = log_k;
            sin.formula = formula;
            this->si_list.push_back(sin);
        }
        if (line.size() == 0)
        {
            row++;
        }
        //printSaturationIndices(this->si_list);
    }

    bool writeAnalisysFile(string path, int analisys_id)
    {
        fstream output_file;

        output_file.open(path, ios::out | ios::app); //open a file to perform write operation using file object
        if (output_file.is_open())
        { //checking whether the file is open

            output_file << "\n";
            output_file << "Analisys " << analisys_id << endl;
            output_file << "\n";

            writeMetadata(output_file);
            writeSolutionComposition(output_file);
            writeDescriptionOfSolution(output_file);
            writeDistributionOfSpecies(output_file);
            writeDistributionOfAlkalinity(output_file);
            writeSaturationIndices(output_file);

            output_file.close(); //close the file object.

            return true;
        }

        return false;
    }

    void writeMetadata(fstream &file)
    {

        file << "--------------------Info Header--------------------" << endl;
        file << endl;
        file << "job_type: " << a.job_type << endl;
        file << "survey: " << a.survey << endl;
        file << "date: " << a.date << endl;
        file << "database: " << a.database << endl;
        file << "phreeqc_version: " << a.phreeqc_version << endl;
        file << "number_run: " << a.run_number << endl;

        file << endl;
        file << "===================================================" << endl;
        file << "--------------------Input file--------------------" << endl;
        file << endl;
        file << "sample_name: " << a.sample_name << endl;
        file << "input_file: " << a.input_file << endl;
        file << "coord_x: " << a.coord_x << endl;
        file << "coord_y: " << a.coord_y << endl;
        file << "EPSG: " << a.epsg << endl;
        file << "timestamp: " << a.timestamp << endl;
        file << endl;
    }

    void writeSolutionComposition(fstream &file)
    {
        file << "--------------------Solution Composition--------------------" << endl;
        file << "\n";
        file.width(20);
        file << left << "ELEMENTS";
        file.width(20);
        file << right << "MOLALITY";
        file.width(20);
        file << right << "MOLES" << endl;
        file << "\n";

        for (int i = 0; i < sc_list.size(); i++)
        {
            file.precision(3);
            file.width(20);
            file << left << sc_list[i].element;
            file.width(20);
            file << scientific << right << stod(sc_list[i].molality);
            file.width(20);
            file << scientific << right << stod(sc_list[i].moles) << endl;
        }
        sc_list.clear();
    }

    void writeDescriptionOfSolution(fstream &file)
    {
        file << "\n";
        file << "--------------------Description of Solution--------------------" << endl;
        file << "\n";

        for (int i = 0; i < des_list.size(); i++)
        {
            file.width(40);
            file << right << des_list[i].key_name << "\t=\t";
            file.width(20);
            file << left << des_list[i].value << endl;
        }
        des_list.clear();
    }

    void writeDistributionOfSpecies(fstream &file)
    {
        file << "\n";
        file << "--------------------Distribution of Species--------------------" << endl;
        file << "\n";
        file.width(15);
        file << left << "\tSPECIES";
        file.width(15);
        file << right << "MOLALITY";
        file.width(15);
        file << right << "ACTIVITY";
        file.width(15);
        file << right << "LOG MOLALITY";
        file.width(15);
        file << right << "LOG ACTIVITY";
        file.width(15);
        file << right << "LOG GAMMA" << endl;
        file << "\n";

        for (int i = 0; i < dis_list.size(); i++)
        {

            if (dis_list[i].activity.size() > 0)
            {
                file << "\t";
            }

            file.width(15);
            file << left << dis_list[i].species;
            file.width(15);
            if (dis_list[i].activity.size() > 0)
            {
                file << scientific << right << stod(dis_list[i].molality);
            }
            else
            {
                file << left << dis_list[i].molality;
            }
            file.width(15);
            if (dis_list[i].activity.size() > 0)
            {
                file << scientific << right << stod(dis_list[i].activity);
            }
            else
            {
                file << left << dis_list[i].activity;
            }
            file.width(15);
            file << right << dis_list[i].log_molality;
            file.width(15);
            file << right << dis_list[i].log_activity;
            file.width(15);
            file << right << dis_list[i].log_gamma << endl;
        }
        dis_list.clear();
    }

    void writeDistributionOfAlkalinity(fstream &file)
    {
        file << "\n";
        file << "--------------------Distribution of Alkalinity--------------------" << endl;
        file << "\n";
        file.width(20);
        file << left << "SPECIES";
        file.width(20);
        file << right << "ALKALINITY";
        file.width(20);
        file << right << "MOLALITY";
        file.width(20);
        file << right << "ALK/MOL" << endl;
        file << "\n";

        for (int i = 0; i < alk_list.size(); i++)
        {
            file.precision(2);
            file.width(20);
            file << left << alk_list[i].species;
            file.width(20);
            file << scientific << right << stod(alk_list[i].alkalinity);
            file.width(20);
            file << scientific << right << stod(alk_list[i].molality);
            file.width(20);
            file << scientific << right << stod(alk_list[i].alk_mol) << endl;
        }
        alk_list.clear();
    }

    void writeSaturationIndices(fstream &file)
    {
        file << "\n";
        file << "--------------------Saturation Indices--------------------" << endl;
        file << "\n";
        file.width(20);
        file << left << "PHASE";
        file.width(20);
        file << right << "SI**";
        file.width(20);
        file << right << "LOG IAP";
        file.width(25);
        file << right << "LOG K(285 K, 1 atm)" << endl;
        file << "\n";

        for (int i = 0; i < si_list.size(); i++)
        {
            file.width(20);
            file << left << si_list[i].phase;
            file.width(20);
            file << right << si_list[i].si;
            file.width(20);
            file << right << si_list[i].log_iap;
            file.width(20);
            file << right << si_list[i].log_k;
            file << "\t\t";
            file.width(25);
            file << left << si_list[i].formula << endl;
        }
        si_list.clear();
    }
};
