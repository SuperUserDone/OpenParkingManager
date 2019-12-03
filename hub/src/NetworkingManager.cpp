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

#include "NetworkingManager.hpp"

NetworkingManager::NetworkingManager()
    : m_listener("6969")
    , m_locate_ticket("LT.*")
    , m_image("MP\n\\d+\n\\d+\n\\d+\n")
{
    m_db = &Database::getInstance();
    m_locate_ticket.set_run_function(std::bind(&NetworkingManager::locate_ticket_callback, this, std::placeholders::_1, std::placeholders::_2));
    m_image.set_run_function(std::bind(&NetworkingManager::image_receive_callback, this, std::placeholders::_1, std::placeholders::_2));

    m_listener.listen_async();
}

void NetworkingManager::image_receive_callback(LouisNet::Socket* sock, const std::string& data)
{
}

void NetworkingManager::locate_ticket_callback(LouisNet::Socket* sock, const std::string& data)
{
    std::string data_args = data.substr(2);
    std::string result = m_db->get_parking_by_ticket(strtoul(data_args.c_str(), NULL, 0));
    sock->send_data((result == "") ? "NULL" : result);
}

NetworkingManager::~NetworkingManager()
{
}