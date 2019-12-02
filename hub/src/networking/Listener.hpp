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

#include <string>
#include <thread>
#include <vector>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "networking/AsyncSocket.hpp"
#include "networking/Socket.hpp"

class Listener {
private:
    std::vector<AsyncSocket*> m_sockets;

    std::thread m_listener_worker_thread;

    void listen_worker();

    AsyncSocket* add_connection(int fd);
    void clean_connections();

    int m_sock_fd;

    std::atomic_bool m_listening = false;

public:
    Listener(const std::string& port);

    void start();
    void listen_async();
    void stop();

    std::vector<Socket*> get_sockets();

    struct {
        Simple::Signal<void(Listener*, AsyncSocket*)> on_accept;
    } signal;

    ~Listener();
};
