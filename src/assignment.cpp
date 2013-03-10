#include <iostream>
using namespace std;
#include "sqlite3.h"

int main(){
	sqlite3* pDb = NULL;
    sqlite3_stmt* query = NULL;
    int ret = 0;
    do // avoid nested if's
    {
        // initialize engine
        if (SQLITE_OK != (ret = sqlite3_initialize()))
        {
            printf("Failed to initialize library: %d\n", ret);
            break;
        }
        // open connection to a DB
        if (SQLITE_OK != (ret = sqlite3_open_v2("test.db", &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
        {
            printf("Failed to open conn: %d\n", ret);
            break;
        }
        // prepare the statement
        if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "CREATE TABLE IF NOT EXISTS Persons(P_Id int primary key autoincrement,LastName varchar(255),FirstName varchar(255),Address varchar(255),City varchar(255))", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "INSERT INTO Persons VALUES(1, '\"thecage\" cage', 'Nickolas', 'Somewhere', 'Amsterdam')", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		        // step to 1st row of data
		if (SQLITE_DONE != (ret = sqlite3_step(query))) // see documentation, this can return more values as success
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "SELECT * FROM Persons", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
        // step to 1st row of data
        if (SQLITE_ROW != (ret = sqlite3_step(query))) // see documentation, this can return more values as success
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
        // ... and print the value of column 0 (expect 2012 here)
        printf("Value from sqlite: %s", sqlite3_column_text(query, 3));     

    } while (false);
    // cleanup
    if (NULL != query) sqlite3_finalize(query);
    if (NULL != pDb) sqlite3_close(pDb);
    sqlite3_shutdown();
	while(true);
    return ret;
}