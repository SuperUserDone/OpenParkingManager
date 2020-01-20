/*
    OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later
   version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <map>
#include <sqlite3.h>
#include <stdint.h>
#include <string>

#include "Data.hpp"

class Database
{
private:
    sqlite3 *m_db;
    std::map<std::string, std::string> m_query_results;
    bool m_query_results_available = false;

public:
    Database();
    ~Database();

    Database(Database const &) = delete;
    void operator=(Database const &) = delete;

    void add_user(User data);
    void add_ticket(Ticket data);

    User get_user(User data);
    Ticket get_ticket(Ticket data);

    void update_user(User data);
    void update_ticket(Ticket data);

    void destroy_ticket(Ticket data);

    std::map<std::string, std::string> query(const std::string &query);

    int callback(void *NotUsed, int argc, char **argv, char **azColName);
    static Database &get()
    {
        static Database instance;
        return instance;
    }
};
