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

#include <networking/Listener.hpp>

Listener::Listener(const std::string& port)
{
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((m_sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(m_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt SO_REUSEADDR");
            return;
        }

        if (bind(m_sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(m_sock_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return;
    }
}

void Listener::start()
{
    if (listen(m_sock_fd, 10) == -1) {
        perror("listen");
    }
    m_listening = true;
    listen_worker();
}

void Listener::listen_async()
{
    m_listener_worker_thread = std::thread(std::bind(&Listener::start, this));
}

void Listener::stop()
{
    close(m_sock_fd);
    if (m_listener_worker_thread.joinable())
        m_listener_worker_thread.join();
}

void Listener::listen_worker()
{
    while (m_listening) {
        clean_connections();
        struct sockaddr_storage their_addr;
        socklen_t sin_size = sizeof their_addr;
        int new_fd = accept(m_sock_fd, (struct sockaddr*)&their_addr, &sin_size);
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            continue;
        } else if (new_fd == -1) {
            perror("accept");
            continue;
        }
        signal.on_accept.emit(this, add_connection(new_fd));
    }
}

AsyncSocket* Listener::add_connection(int fd)
{
    Socket* socket = new Socket();
    socket->set_connection_fd(fd);
    AsyncSocket* asocket = new AsyncSocket(socket);
    asocket->start();
    m_sockets.push_back(asocket);
    return asocket;
}

std::vector<Socket*> Listener::get_sockets()
{
    std::vector<Socket*> sockets;
    for (auto a : m_sockets) {
        sockets.push_back(a->getSocket());
    }
    return sockets;
}

void Listener::clean_connections()
{
    for (int i = 0; i < m_sockets.size(); i++) {
        if (!m_sockets[i]->getSocket()->is_connected()) {
            m_sockets.erase(m_sockets.begin() + i);
            clean_connections();
            return;
        }
    }
}

Listener::~Listener()
{
}
