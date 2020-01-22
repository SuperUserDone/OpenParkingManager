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
#include "Database.hpp"

#include <json.hpp>

#include <functional>
#include <inttypes.h>
#include <iostream>
#include <sstream>

using namespace std::placeholders;

std::function<int(void *, int, char **, char **)> callback_cpp;

static int callback_c(void *NotUsed, int argc, char **argv, char **azColName)
{
    return callback_cpp(NotUsed, argc, argv, azColName);
}

int Database::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        // printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        m_query_results[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }
    m_query_results_available = true;
    // printf("\n");
    return 0;
}

Database::Database()
{
    int rc;
    rc = sqlite3_open("database.db", &m_db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        return;
    }
    callback_cpp = std::bind(&Database::callback, this, _1, _2, _3, _4);
}

Database::~Database() { sqlite3_close(m_db); }

std::map<std::string, std::string> Database::query(const std::string &query)
{
    std::cout << query << std::endl;
    m_query_results.clear();
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(m_db, query.c_str(), callback_c, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return std::map<std::string, std::string>();
    }
    std::map<std::string, std::string> results = m_query_results;
    return results;
}

void Database::add_user(User data)
{
    std::stringstream ss;

    nlohmann::json owns_json(data.Owns);

    ss << "INSERT INTO 'Users' ('Email', 'Hash', 'Owns', 'ID') VALUES "
       << "('" << data.Email << "', '" << data.Password << "', '"
       << owns_json.dump() << "', '" << data.ID << "');";

    query(ss.str());
}

void Database::add_ticket(Ticket data)
{
    std::stringstream ss;

    ss << "INSERT INTO 'Parking0' ('License', 'Ticket', 'Paid') VALUES "
       << "('" << data.License << "', '" << data.Ticket << "',"
       << "false"
       << ");";

    query(ss.str());
}

User Database::get_user(User data)
{
    std::stringstream ss;

    ss << "SELECT * FROM 'Users' WHERE ";

    if (data.Password == "")
    {
        User temp;
        temp.Email = "NULL";
        temp.Password = "NULL";
        temp.ID = "NULL";
        return temp;
    }

    if (data.Email != "")
    {
        ss << "Email = '" << data.Email << "' AND ";
    }

    if (data.ID != "")
    {
        ss << "ID = '" << data.ID << "' AND ";
    }

    ss << "Hash = '" << data.Password << "'";

    std::map<std::string, std::string> q_return = query(ss.str());

    if (q_return.empty())
    {
        User temp;
        temp.Email = "NULL";
        temp.Password = "NULL";
        temp.ID = "NULL";
        return temp;
    }

    User user_values;

    if (q_return["Owns"] != "NULL")
    {
        nlohmann::json owns_json = nlohmann::json::parse(q_return["Owns"]);

        for (std::string str : owns_json)
            user_values.Owns.push_back(str);
    }

    user_values.Email = q_return["Email"];
    user_values.Password = q_return["Hash"];
    user_values.Priv = stoi(q_return["Priv"]);
    user_values.ID = q_return["ID"];

    return user_values;
}

Ticket Database::get_ticket(Ticket data)
{
    std::stringstream ss;

    ss << "SELECT * from 'Parking0' WHERE ";

    if (data.Ticket != "")
    {
        ss << "Ticket = '" << data.Ticket << "' AND";
    }
    if (data.Parking != "")
    {
        ss << "Parking = '" << data.Parking << "' AND";
    }
    if (data.License != "")
    {
        ss << "License = '" << data.License << "' AND";
    }

    ss << " 1=1";

    std::map<std::string, std::string> q_return = query(ss.str());

    if (q_return.empty())
    {
        Ticket ticket_values;

        ticket_values.License = "NULL";
        ticket_values.Parking = "NULL";
        ticket_values.Ticket = "NULL";
        ticket_values.paid = false;

        return ticket_values;
    }

    Ticket ticket_values;

    ticket_values.License = q_return["License"];
    ticket_values.Parking = q_return["Parking"];
    ticket_values.Ticket = q_return["Ticket"];
    if (q_return["Paid"] == "True")
        ticket_values.paid = true;

    if (q_return["Paid"] == "False")
        ticket_values.paid = false;

    return ticket_values;
}

void Database::update_user(User data)
{
    std::stringstream ss;

    nlohmann::json owns_json(data.Owns);

    ss << "UPDATE 'Users' SET "
       << "Email = '" << data.Email << "', "
       << "Hash = '" << data.Password << "', "
       << "Priv = '" << data.Priv << "', "
       << "Owns = '" << owns_json.dump() << "'"
       << "WHERE ID = '" << data.ID << "'";

    query(ss.str());
}

void Database::update_ticket(Ticket data)
{
    std::stringstream ss;

    ss << "UPDATE 'Parking0' SET "
       << "Ticket = '" << data.Ticket << "', "
       << "License = '" << data.License << "', "
       << "Paid = " << (data.paid ? "TRUE" : "FALSE") << ", "
       << "Parking = '" << data.Parking << "'";
    query(ss.str());
}

void Database::destroy_ticket(Ticket data) {}
