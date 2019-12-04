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
#include <functional>
#include <fstream>

#include <LouisNet.hpp>

#include "Database.hpp"

class NetworkingManager {
private:
    LouisNet::Listener m_listener;
    LouisNet::PacketListener m_locate_ticket;
    LouisNet::PacketListener m_image;

    Database* m_db;

public:
    NetworkingManager();

    void image_receive_callback(LouisNet::Socket* sock, const std::string& data);
    void locate_ticket_callback(LouisNet::Socket* sock, const std::string& data);

    ~NetworkingManager();
};
