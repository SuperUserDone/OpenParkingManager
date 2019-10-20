/*
    OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Database.hpp"
#include <iostream>
#include <sstream>
#include <inttypes.h>
#include <functional>
using namespace std::placeholders;


std::function<int(void*, int, char**, char**)> callback_cpp;

int callback_c(void *NotUsed, int argc, char **argv, char **azColName)
{
    return callback_cpp(NotUsed, argc, argv, azColName);
}

int Database::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for(int i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        m_query_results[azColName[i]] = argv[i];
    }
    m_query_results_available = true;
    printf("\n");
    return 0;
}

Database::Database()
{
    int rc;
    rc = sqlite3_open("database.db", &m_db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        return;
    }
    callback_cpp = std::bind(&Database::callback, this, _1, _2, _3, _4);
}

Database::~Database()
{
    sqlite3_close(m_db);
}

std::map<std::string, std::string> Database::query(std::string query)
{
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(m_db, query.c_str(), callback_c, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    std::cout << query << std::endl;
    return m_query_results;
}

std::string Database::get_parking_by_ticket(uint64_t ticket)
{
    std::stringstream querys;
    querys << "SELECT Parking FROM Parking0 WHERE Ticket = " << ticket;
    return query(querys.str())["Parking"];
}

void Database::store_vehicle(std::string license, uint64_t ticket)
{
    std::stringstream querys;
    querys << "DELETE FROM Parking0 WHERE License = '" << license << "'";
    query(querys.str());

    querys.str("");

    querys << "INSERT INTO Parking0 VALUES ('" << ticket << "', '" << license << "', NULL)";
    query(querys.str());
}

void Database::store_parking(std::string license, std::string parking)
{
    std::stringstream querys;
    querys << "UPDATE Parking0 SET Parking = '" << parking << "' WHERE License = '" << license << "'";
    query(querys.str());
}

void Database::destroy_ticket(uint64_t ticket)
{
    std::stringstream querys;
    querys << "DELETE FROM Parking0 WHERE Ticket = '" << ticket << "'";
    query(querys.str());
}