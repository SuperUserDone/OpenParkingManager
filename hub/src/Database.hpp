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
#pragma once

#include <sqlite3.h>
#include <stdint.h>
#include <string>
#include <mutex>
#include <map>

class Database
{
private:
    sqlite3 *m_db;
    std::map<std::string, std::string> m_query_results;
    bool m_query_results_available = false;

    std::mutex m_query_lock;

public:
    Database();
    ~Database();

    Database(Database const&) = delete;
    void operator=(Database const&) = delete;

    std::string get_parking_by_ticket(const std::string& ticket);

    std::string get_ticket_by_license(const std::string& license);

    void store_vehicle(const std::string& license, const std::string& ticket);
    void store_parking(const std::string& license, const std::string& parking);
    void remove_parking(const std::string& parking);
    void destroy_ticket(const std::string& ticket);
    std::map<std::string, std::string> query(const std::string& query);

    int callback(void *NotUsed, int argc, char **argv, char **azColName);
    static Database& getInstance()
    {
        static Database instance;
        return instance;
    }
};
