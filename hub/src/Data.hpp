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

#include <string>
#include <vector>

#include <json.hpp>

struct Ticket
{
    std::string Ticket = "";
    std::string License = "";
    std::string Parking = "";
    bool paid = false;
};

inline void to_json(nlohmann::json &j, const Ticket &t)
{
    j = nlohmann::json{{"ticket", t.Ticket},
                       {"parking", t.Parking},
                       {"license", t.License},
                       {"paid", t.paid}};
}

inline void from_json(const nlohmann::json &j, Ticket &t) {}

struct User
{
    std::string Email = "";
    std::string Password = "";
    std::string ID = "";

    std::vector<std::string> Owns = {};

    int Priv = 1;
};
