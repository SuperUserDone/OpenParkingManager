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
#include <functional>

#include <ReadLicensePlates.hpp>
#include <base64.hpp>
#include <digestpp.hpp>
#include <opencv2/opencv.hpp>

#include "NetworkingManager.hpp"

using namespace std::placeholders;

bool NetworkingManager::auth(const std::string &password, const std::string &ID)
{
    User temp;
    temp.Password = password;
    temp.ID = ID;

    User user_data = m_db->get_user(temp);

    if (user_data.Email == "NULL")
        return false;

    return true;
}

NetworkingManager::NetworkingManager()
{
    m_db = &Database::get();

    AddressFunction temp = std::bind(&NetworkingManager::root, this, _1, _2);
    m_server.Get("/", temp);

    temp = std::bind(&NetworkingManager::stop, this, _1, _2);
    m_server.Get("/stop", temp);

    temp = std::bind(&NetworkingManager::entry_issue_ticket, this, _1, _2);
    m_server.Post("/entry/issue", temp);

    temp = std::bind(&NetworkingManager::user_login, this, _1, _2);
    m_server.Post("/user/login", temp);

    temp = std::bind(&NetworkingManager::user_add_ticket, this, _1, _2);
    m_server.Post(R"(/user/(\S+)/ticket/add/(\S+))", temp);

    temp = std::bind(&NetworkingManager::user_get_tickets, this, _1, _2);
    m_server.Post(R"(/user/(\S+)/ticket/get)", temp);

    temp = std::bind(&NetworkingManager::user_pay_ticket, this, _1, _2);
    m_server.Post(R"(/user/(\S+)/ticket/(\S+)/pay)", temp);
}

void NetworkingManager::start() { m_server.listen("0.0.0.0", 8080); }

void NetworkingManager::stop(const httplib::Request &, httplib::Response &)
{
    m_server.stop();
}

void NetworkingManager::root(const httplib::Request &req,
                             httplib::Response &res)
{
    res.set_content("up", "text/plain");
}

void NetworkingManager::entry_issue_ticket(const httplib::Request &req,
                                           httplib::Response &res)
{
    res.set_content("Hello World!", "text/json");
}

void NetworkingManager::user_login(const httplib::Request &req,
                                   httplib::Response &res)
{
    if (req.has_file("email") && req.has_file("password"))
    {
        User temp;
        temp.Email = req.get_file_value("email").content;
        temp.Password = req.get_file_value("password").content;

        User user_data = m_db->get_user(temp);

        if (user_data.ID == "NULL")
        {
            res.status = 401;
            return;
        }

        res.set_content(user_data.ID, "text/plain");
    }
    else
    {
        res.status = 401;
    }
}

void NetworkingManager::user_add_ticket(const httplib::Request &req,
                                        httplib::Response &res)
{
    if (!req.has_file("password"))
    {
        res.status = 401;
        return;
    }

    std::string user = req.matches[1];
    std::string ticket = req.matches[2];
    std::string password = req.get_file_value("password").content;

    Ticket temp_ticket;
    temp_ticket.Ticket = ticket;

    temp_ticket = m_db->get_ticket(temp_ticket);

    if (temp_ticket.Ticket == "NULL" || !auth(password, user))
    {
        res.status = 401;
        return;
    }

    User temp_user;
    temp_user.ID = user;
    temp_user.Password = password;

    temp_user = m_db->get_user(temp_user);

    for (std::string ticket_number : temp_user.Owns)
    {
        if (ticket_number == ticket)
            return;
    }

    temp_user.Owns.push_back(temp_ticket.Ticket);

    m_db->update_user(temp_user);
}

void NetworkingManager::user_get_tickets(const httplib::Request &req,
                                         httplib::Response &res)
{
    std::string user = req.matches[1];
    std::string password = req.get_file_value("password").content;

    if (!auth(password, user))
    {
        res.status = 401;
        return;
    }

    User temp_user;
    temp_user.ID = user;
    temp_user.Password = password;

    temp_user = m_db->get_user(temp_user);

    nlohmann::json json_back;

    json_back["tickets"] = nlohmann::json::value_t::array;
    for (std::string ticket_number : temp_user.Owns)
    {
        Ticket temp_ticket;
        temp_ticket.Ticket = ticket_number;

        temp_ticket = m_db->get_ticket(temp_ticket);

        if (temp_ticket.Ticket == "NULL")
            continue;

        json_back["tickets"].push_back(temp_ticket);
    }

    res.set_content(json_back.dump(4), "text/json");
}

void NetworkingManager::user_pay_ticket(const httplib::Request &req,
                                        httplib::Response &res)
{
    std::string user = req.matches[1];
    std::string ticket_number = req.matches[2];
    std::string password = req.get_file_value("password").content;

    if (!auth(password, user))
    {
        res.status = 401;
        return;
    }

    Ticket temp_ticket;
    temp_ticket.Ticket = ticket_number;

    temp_ticket = m_db->get_ticket(temp_ticket);

    if (temp_ticket.Ticket == "NULL")
    {
        res.status == 400;
        return;
    }

    temp_ticket.paid = true;

    m_db->update_ticket(temp_ticket);
}

void NetworkingManager::data_get_image(const httplib::Request &req,
                                       httplib::Response &res)
{
}

void NetworkingManager::node_store_parking(const httplib::Request &req,
                                           httplib::Response &res)
{
}

void NetworkingManager::payment_get_parking(const httplib::Request &req,
                                            httplib::Response &res)
{
}
void NetworkingManager::payment_pay_ticket(const httplib::Request &req,
                                           httplib::Response &res)
{
}

NetworkingManager::~NetworkingManager() {}