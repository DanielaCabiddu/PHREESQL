#include <string>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <ctype.h>
using namespace std;

#ifndef UTILS
#define UTILS
#pragma once

const std::string WHITESPACE = " \n\r\t\f\v";

inline std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

inline std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

inline std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

inline void removeHeaders(ifstream &file, int row)
{
    string line;
    for (int i = 0; i < row; i++)
    {
        getline(file, line);
    };
}

inline void removeHeaders1(ifstream &file, string first, int &row)
{
    string line;

    while (strstr(line.c_str(), first.c_str()) == NULL)
    {
        getline(file, line);
        row++;
        if (line.length() > 0 && strstr(line.c_str(), first.c_str()) == NULL && strstr(line.c_str(), "Log") == NULL)
        {
            cerr << "*** Warning: Line " << row << " " << line << " ***" << endl;
        }
    }
    getline(file, line); //remove empty line
    row++;
}

inline std::string toUpperString(string str)
{
    string aux = "";
    for (int i = 0; i < strlen(str.c_str()); i++)
    {
        aux += toupper(str[i]);
    }
    return aux;
}

inline std::string valueColName(string table_name)
{
    if (strcmp(toUpperString(table_name).c_str(), "DESCRIPTION_OF_SOLUTION") == 0)
    {
        return "KEY";
    }
    else if (strcmp(toUpperString(table_name).c_str(), "DISTRIBUTION_OF_ALKALINITY") == 0 || strcmp(toUpperString(table_name).c_str(), "DISTRIBUTION_OF_SPECIES") == 0)
    {
        return "SPECIES";
    }
    else if (strcmp(toUpperString(table_name).c_str(), "SATURATION_INDICES") == 0)
    {
        return "PHASE";
    }
    else if (strcmp(toUpperString(table_name).c_str(), "SOLUTION_COMPOSITION") == 0)
    {
        return "ELEMENTS";
    }
    else if (strcmp(toUpperString(table_name).c_str(), "SOLUTION_INPUT") == 0)
    {
        return "UNIT";
    }
    else
    {
        return "";
    }
}

#endif
