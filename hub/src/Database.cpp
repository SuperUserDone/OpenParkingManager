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

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for(int i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

Database::Database()
{
    int rc;
    rc = sqlite3_open("parking.db", &m_db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        return;
    }
}

Database::~Database()
{
    sqlite3_close(m_db);
}

void Database::query(std::string query)
{
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(m_db, query.c_str(), callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
}