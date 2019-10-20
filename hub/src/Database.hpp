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
#include <string>
#include <stdint.h>

class Database
{
private:
    sqlite3 *m_db;
public:
    Database();
    ~Database();
    
    std::string get_parking_by_ticket(uint64_t ticket);

    void store_parking(std::string licence, std::string parking);
    void delete_parking(std::string parking);
    void destroy_ticket(uint64_t ticket);
    void query(std::string query);
};
