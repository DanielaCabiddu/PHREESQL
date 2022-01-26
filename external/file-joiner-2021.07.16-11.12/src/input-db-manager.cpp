#include "db-manager.cpp"

class InputDBManager : public DBManager
{

public:
    InputDBManager(sqlite3 *db) : DBManager(db) {}

    ~InputDBManager () {}

    bool isInputEmpty()
    {
        int num_tables;
        query = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='SOLUTION_INPUT';";
        rc = sqlite3_exec(db, query.c_str(), emptyDBCallback, &num_tables, &err_message);
        this->queryResult(rc, "isInputEmpty");
        return num_tables == 0;
    }

//    void createMetadataTable()
//    {
//        query = "CREATE TABLE IF NOT EXISTS METADATA "
//                "( ID INTEGER PRIMARY KEY NOT NULL,"
//                "DATABASE TEXT,"
//                "ANALYSIS_ID INTEGER NOT NULL,"
//                "TITLE TEXT,"
//                "SOLUTION TEXT,"
//                "CONSTRAINT ANALYSIS_ID "
//                "FOREIGN KEY (ANALYSIS_ID) REFERENCES ANALYSIS(ID) );";
//        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

//        this->queryResult(rc, "Create table metadata");
//    }

    void createSolutionInputTable()
    {
        this->err_message = 0;
        query = "CREATE TABLE IF NOT EXISTS SOLUTION_INPUT "
                "( ID INTEGER PRIMARY KEY NOT NULL,"
                "ANALYSIS_ID INTEGER NOT NULL,"
                "SOLUTION_NUMBER TEXT,"
                "PARAM TEXT, VALUE TEXT,"
                "CONSTRAINT ANALYSIS_ID "
                "FOREIGN KEY (ANALYSIS_ID) REFERENCES ANALYSIS(ID) );";
        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);

        this->queryResult(rc, "Create table solution_input");
    }

//    void insertMetadata(metadata meta, int a_id)
//    {
//        query = "INSERT OR REPLACE INTO METADATA (ANALYSIS_ID, DATABASE, TITLE, SOLUTION) VALUES ('" +
//                to_string(a_id) + "', '" +
//                meta["DATABASE"] + "', '" +
//                meta["TITLE"] + "', '" +
//                meta["SOLUTION"] + "');";

//        rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
//        this->queryResult(rc, "insert or replace metadata");
//    }

    void insertSolutionInputs(vector<SolutionInput> inputs, int a_id)
    {
        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

        for (int i = 0; i < inputs.size(); i++)
        {
            query = "INSERT OR REPLACE INTO SOLUTION_INPUT (ANALYSIS_ID, SOLUTION_NUMBER, PARAM, VALUE) VALUES ('" +
                    to_string(a_id) + "', '" +
                    inputs[i].solution_number + "', '" +
                    inputs[i].unit + "', '" +
                    inputs[i].ppm + "');";

            rc = sqlite3_exec(db, query.c_str(), 0, 0, &err_message);
            this->queryResult(rc, "insert or replace solution_input");
        }

        sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
    }

    void selectValuesFromMetadata(metadata &meta, int ANALYSIS_num)
    {
        char **res;
        int row, column;

        query = "SELECT * FROM METADATA WHERE ANALYSIS_ID='" + to_string(ANALYSIS_num) + "';";
        rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
        this->queryResult(rc, "selecting data from metadata");

        for (int i = 0; i < column; i++)
        {
            meta.insert(pair<string, string>(res[i], res[i + column]));
        }
        sqlite3_free_table(res);
    }

    void selectValuesFromSolution(vector<SolutionInput> &inputs, int ANALYSIS_num)
    {
        char **res;
        int row, column;
        SolutionInput input;

        query = "SELECT * FROM SOLUTION_INPUT WHERE ANALYSIS_ID='" + to_string(ANALYSIS_num) + "';";
        rc = sqlite3_get_table(db, query.c_str(), &res, &row, &column, &err_message);
        this->queryResult(rc, "selecting data from solution_input");

        for (int i = 5; i < (row + 1) * column; i = i + 5)
        {
            input.analisys_id = atoi(res[i + 1]);
            input.solution_number = res[i + 2];
            input.unit = res[i + 3];
            input.ppm = res[i + 4];
            inputs.push_back(input);
        }
        sqlite3_free_table(res);
    }
};
