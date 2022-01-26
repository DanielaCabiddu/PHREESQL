
#include <sqlite3.h>
#include <algorithm>
#include <iterator>
#include "models.h"
using namespace std;

//const char *DB_NAME = "matrac.db";
#ifndef DBM
#define DBM
#pragma once

static int count_rows(void *param, int argc, char **argv, char **azColName)
{
    int *value = static_cast<int *>(param);

    *value = atoi(argv[0]);

    return 0;
}

static int emptyDBCallback(void *param, int argc, char **argv, char **azColName)
{
    int *value = static_cast<int *>(param);

    *value = atoi(argv[0]);

    return 0;
}

class DBManager
{
protected:
    sqlite3 *db;
    char *err_message;
    int rc;
    string query;


public:

    const string metadata_table_name = "METADATA";

    DBManager(sqlite3 *db)
    {
        this->db = db;
    }

    ~DBManager () {}

    void queryResult(int rc, string message)
    {
        this->err_message = 0;
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error on %s: %s (%s)\n", message.c_str(), err_message, sqlite3_errstr(rc));
            sqlite3_free(err_message);
        }
        else
        {
            //fprintf(stdout, message.c_str());
        }
    }

    bool isDBEmpty()
    {
        int num_tables;
        query = "SELECT count(*) FROM sqlite_master WHERE type='table';";
        rc = sqlite3_exec(db, query.c_str(), emptyDBCallback, &num_tables, &err_message);
        this->queryResult(rc, "isDBEmpty");
        return num_tables == 0;
    }

    int getNumRows(string table_name)
    {
        int num;
        query = "SELECT count(*) FROM " + table_name + ";";
        rc = sqlite3_exec(db, query.c_str(), count_rows, &num, &err_message);
        return num;
    }
};

#endif
