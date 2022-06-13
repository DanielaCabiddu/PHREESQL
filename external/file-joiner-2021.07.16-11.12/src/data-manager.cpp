#include "db-manager.cpp"

#include <string>
#include <vector>
#include <float.h>
#include <limits.h>
#include "utils.h"
using namespace std;

class DataManager : public DBManager
{
public:
    DataManager(sqlite3 *db) : DBManager(db) {}

    ~DataManager () {}

    std::vector<std::vector<std::pair<std::string, std::string>>> getMetadata()
    {
        int row, column;
        std::vector<std::vector<std::pair<std::string, std::string>>> res;

        std::string query = "SELECT * FROM " + metadata_table_name + ";";
//        std::cout << query << std::endl;
        rc = sqlite3_exec(db, query.c_str(), printDBCallback, &res, &err_message);
        this->queryResult(rc, "data2epsg");

//        for (uint i=0; i < res.size(); i++)
//        {
//            std::cout << "[" << i << "]" << res.at(i).size() << std::endl;

//            for (const std::pair<std::string, std::string> &p : res.at(i))
//            {
//                std::cout << p.first << " : " << p.second << std::endl;
//            }
//        }

        return res;
    }

    vector<vector<string> > getData(string table_name, string value, string column_name, string timestamp)
    {
        char **res;
        int row, column;

        query = "SELECT " + toUpperString(column_name) + ", TIMESTAMP FROM " + toUpperString(table_name) + " JOIN ANALISYS ON ANALISYS_ID LIKE ANALISYS.ID WHERE " + "ANALISYS.TIMESTAMP>=\"" + timestamp + "\" AND " + valueColName(table_name) + "=\"" + value + "\" ORDER BY TIMESTAMP;";
        rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
        this->queryResult(rc, "getData");

        vector<vector<string> > data;
        vector<string> elems;
        for (int i = 2; i < (row + 1) * column; i = i + 2)
        {
            elems.clear();
            elems.push_back(res[i]);
            elems.push_back(res[i + 1]);
            data.push_back(elems);
        };

        sqlite3_free_table(res);
        return data;
    }

    bool writeToFile(string path, vector<vector<string> > data)
    {
        fstream output_file;
        output_file.open(path, ios::out | ios::trunc);
        if (output_file.is_open())
        {
            for (int i = 0; i < data.size(); i++)
            {
                output_file << data[i][1] << " " << data[i][0] << endl;
            }
            return true;
        }

        return false;
    }
};
