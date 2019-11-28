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

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "networking/Packet.hpp"

class Socket
{
private:
    std::string m_address;
    std::string m_port;

    bool connected = false;

    int m_socket_fd = 0;
    
public:
    Socket();
    Socket(const std::string &address, const std::string &port);

    bool connect_socket();

    void set_address(const std::string &address);
    void set_port(const std::string &port);
    void set_connection_fd(int fd);

    std::string get_address();
    std::string get_port();

    bool send_data(const std::string &data);
    bool send_data(Packet data);
    std::string receive_data(int max_size = 1024);


    void disconnect();

    ~Socket();
    
};


