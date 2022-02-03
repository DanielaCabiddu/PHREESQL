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

    bool writeFile(int i, string directory)
    {
        fstream output_file;

        output_file.open(directory + "/dump_input_" + to_string(i) + ".txt", ios::out); //open a file to perform write operation using file object
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
