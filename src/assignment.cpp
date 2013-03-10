#include <iostream>
using namespace std;
#include "sqlite3.h"

int main(){
	sqlite3* pDb = NULL;
    sqlite3_stmt* query = NULL;
    int ret = 0;

    do
    {
        // Initialize engine
        if (SQLITE_OK != (ret = sqlite3_initialize()))
        {
            printf("Failed to initialize library: %d\n", ret);
            break;
        }
        // Open connection to a DB
        if (SQLITE_OK != (ret = sqlite3_open_v2("test.db", &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
        {
            printf("Failed to open conn: %d\n", ret);
            break;
        }

        /*
		*
		* Creating tables.
		*
		*/
        if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, 
			"CREATE TABLE IF NOT EXISTS Cities(City_Id INTEGER PRIMARY KEY AUTOINCREMENT, City_Name varchar(255))",
			-1, &query, NULL)))
        {
            printf("Failed to create table: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_DONE != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }

		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb,
			"CREATE TABLE IF NOT EXISTS Persons(Person_Id INTEGER PRIMARY KEY AUTOINCREMENT, Last_Name varchar(255), First_Name varchar(255), City varchar(30) REFERENCES Cities(City_Name))",
			-1, &query, NULL)))
        {
            printf("Failed to create table: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_DONE != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }

		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb,
			"CREATE TABLE IF NOT EXISTS Houses(House_Id INTEGER PRIMARY KEY AUTOINCREMENT, Owner varchar(30) REFERENCES Persons(First_Name))",
			-1, &query, NULL)))
        {
            printf("Failed to create table: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_DONE != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }

		/*
		*
		* Inserting values.
		*
		*/
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "INSERT INTO Cities(City_Name) VALUES('Purmerend')", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_DONE != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }

		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "INSERT INTO Persons(Last_Name, First_Name, City) VALUES('Bulsing', 'Rene', 'Purmerend')", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_DONE != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }

		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "INSERT INTO Houses(Owner) VALUES('Rene')", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		if (SQLITE_DONE != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
		
		/*
		*
		* Finalizing queues
		*
		*/
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(pDb, "SELECT First_Name FROM Persons WHERE Persons.City='Purmerend'", -1, &query, NULL)))
        {
            printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }
        if (SQLITE_ROW != (ret = sqlite3_step(query)))
        {
            printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(pDb));
            break;
        }

		printf("Value from sqlite: %s", sqlite3_column_text(query, 0)); 

    } while (false);

    // Clean-up
    if (NULL != query) sqlite3_finalize(query);
    if (NULL != pDb) sqlite3_close(pDb);
    sqlite3_shutdown();
	
	// Infinite loop for testing purposes.
	while(true);
    return ret;
}