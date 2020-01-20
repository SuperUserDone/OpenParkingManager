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
#include <filesystem>
#include <fstream>
#include <functional>

#include <httplib.h>

#include "Database.hpp"

using AddressFunction =
    std::function<void(const httplib::Request &req, httplib::Response &res)>;

class NetworkingManager
{
private:
    Database *m_db;

    httplib::Server m_server;

    bool auth(const std::string &password, const std::string &ID);

public:
    NetworkingManager();

    void start();
    void stop(const httplib::Request &, httplib::Response &);

    void root(const httplib::Request &req, httplib::Response &res);

    void entry_issue_ticket(const httplib::Request &req,
                            httplib::Response &res); // /entry/issue

    void user_login(const httplib::Request &req, httplib::Response &res);
    void user_add_ticket(const httplib::Request &req, httplib::Response &res);
    void user_get_tickets(const httplib::Request &req, httplib::Response &res);
    void user_pay_ticket(const httplib::Request &req, httplib::Response &res);

    void data_get_image(const httplib::Request &req, httplib::Response &res);

    void node_store_parking(const httplib::Request &req,
                            httplib::Response &res);

    void payment_get_parking(const httplib::Request &req,
                             httplib::Response &res);
    void payment_pay_ticket(const httplib::Request &req,
                            httplib::Response &res);

    ~NetworkingManager();
};
