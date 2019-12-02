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
#include <atomic>
#include <thread>

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>

#include "networking/Packet.hpp"

#include "SimpleSignal.h"

class Socket {
protected:
    std::string m_address;
    std::string m_port;

    std::atomic_int m_socket_fd = 0;
    std::atomic_bool m_connected = false;
public:
    Socket();
    Socket(const std::string& address, const std::string& port);

    bool connect_socket();

    void set_address(const std::string& address);
    void set_port(const std::string& port);
    void set_connection_fd(int fd);

    std::string get_address();
    std::string get_port();

    bool send_data(const std::string& data);
    bool send_data(Packet data);
    std::string receive_data(int max_size = 1024, bool wait = true);
    bool is_connected();

    void disconnect();

    struct {
        Simple::Signal<void(Socket*, const std::string&, const std::string&, const int&)> on_connect;
        Simple::Signal<void(Socket*, const std::string&)> on_packet_receive;
        Simple::Signal<void(Socket*, const std::string&)> on_packet_send;
        Simple::Signal<void(Socket*)> on_disconnect;
    } signal;

    ~Socket();
};
