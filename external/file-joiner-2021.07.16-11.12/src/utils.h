#include <string>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
using namespace std;

#ifndef UTILS
#define UTILS
#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

const std::string WHITESPACE = " \n\r\t\f\v";

inline std::string separator ()
{
    std::string separator;

    #ifdef _WIN32
    separator = "\\";
    #else
    separator = "/";
    #endif

    return separator;
}

inline bool createDir (const std::string folder)
{
    std::cout << "Creating folder " << folder << std::endl;

    mode_t nMode = 0770; // UNIX style permissions
    int nError = 0;
    #if defined(_WIN32)
      nError = _mkdir(folder.c_str()); // can be used on Windows
    #else
      nError = mkdir(folder.c_str(),nMode); // can be used on non-Windows
    #endif
    if (nError != 0)
    {
      std::cerr << "Error creating folder " << folder << std::endl;
      return false;
    }

    return true;
}

inline bool dirExists(const std::string path)
{
    struct stat info;

    if(stat( path.c_str(), &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

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
