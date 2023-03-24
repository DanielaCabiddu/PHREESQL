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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "models.h"
#include "utils.h"
using namespace std;

class InputReaderWriter
{
public:
    vector<SolutionInput> input_list;
    metadata meta;

    // il path è il path del database sqlite da aprire
    InputReaderWriter() {}

    bool readFile(string path)
    {
        string solution;
        fstream input_file;

        input_file.open(path, ios::in); //open a file to perform read operation using file object

        if (input_file.is_open())
        { //checking whether the file is open
            string line;

            while (getline(input_file, line))
            { //read data from file object and put it into string.
                istringstream iss(line);
                string key, value;
                if (!(iss >> key >> value))
                {
                    if (key.compare("END") == 0)
                    {
                        break;
                    }
                }

                if (key.rfind("#", 0) == 0)
                {
                    continue;
                }

                if (key.compare("DATABASE") == 0)
                {
                    meta.insert(pair<string, string>(key, line.substr(line.find(key) + key.size(), line.size())));
                }
                else if (key.compare("TITLE") == 0)
                {
                    meta.insert(pair<string, string>(key, line.substr(line.find(key) + key.size(), line.size())));
                }
                else if (key.compare("SOLUTION") == 0)
                {
                    meta.insert(pair<string, string>(key, value));
                    solution = value;
                }
                else
                {
//                    if (key.compare("units") != 0)
//                    {
                        SolutionInput input;
                        input.unit = trim(key);
                        input.ppm = trim(line.substr(line.find(key) + key.size(), line.size()));
                        input.solution_number = solution;
                        input_list.push_back(input);
//                    }
                }
            }
            input_file.close(); //close the file object.

            if (meta.find("TITLE") == meta.end())
                meta.insert(pair<string, string>("TITLE", ""));

            if (meta.find("SOLUTION") == meta.end())
                meta.insert(pair<string, string>("SOLUTION", ""));

            return true;
        }
        return false;
    }

    bool writeFile(int i, string file_path)
    {
        fstream output_file;

        output_file.open(file_path, ios::out); //open a file to perform write operation using file object
        if (output_file.is_open())
        { //checking whether the file is open

            output_file << "DATABASE " << meta["DATABASE"] << endl;
            output_file << "TITLE " << meta["TITLE"] << endl;
            output_file << "SOLUTION " << meta["SOLUTION"] << endl;

//            output_file.width(15);
//            output_file << left << "\tunits";
//            output_file.width(15);
//            output_file << left << "ppm" << endl;

            for (int i = 0; i < input_list.size(); i++)
            {
                output_file << "\t";
                output_file.width(15);
                output_file << left << input_list[i].unit;
                output_file.width(15);
                output_file << left << input_list[i].ppm << endl;
            }

            output_file << "END" << endl;
            output_file.close(); //close the file object.
            input_list.clear();
            return true;
        }
        return false;
    }
};
