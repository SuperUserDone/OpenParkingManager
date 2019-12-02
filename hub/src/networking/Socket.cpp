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
#include "networking/Socket.hpp"

namespace LouisNet {
Socket::Socket()
{
}

Socket::Socket(const std::string& address, const std::string& port)
    : m_address(address)
    , m_port(port)
{
    m_connected = false;
}

bool Socket::connect_socket()
{
    struct addrinfo hints;
    struct addrinfo* servinfo;
    struct addrinfo* p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv;
    if ((rv = getaddrinfo(m_address.c_str(), m_port.c_str(), &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        ;
        return false;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((m_socket_fd = socket(p->ai_family, p->ai_socktype,
                 p->ai_protocol))
            == -1) {
            perror("Socket: Creation error");
            continue;
        }
        if (connect(m_socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(m_socket_fd);
            perror("Socket: Connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        std::cout << "Socket: Failed to connect" << std::endl;
        return false;
    }

    freeaddrinfo(servinfo);
    signal.on_connect.emit(this, m_address, m_port, m_socket_fd);
    m_connected = true;
    return true;
}

void Socket::set_address(const std::string& address)
{
    m_address = address;
}

void Socket::set_port(const std::string& port)
{
    m_port = port;
}

void Socket::set_connection_fd(int fd)
{
    m_socket_fd = fd;
    m_connected = true;
    signal.on_connect.emit(this, m_address, m_port, m_socket_fd);
}

std::string Socket::get_address()
{
    return m_address;
}

std::string Socket::get_port()
{
    return m_port;
}

bool Socket::send_data(Packet data)
{
    if (m_connected) {
        if (data.get_meta_package().size() > data.get_maxsize()) {
            std::cout << "Meta package size larger than maxsize! (" << data.get_meta_package().size() << " > " << data.get_maxsize() << ") Please increase maxsize to (preferably) a power of two!" << std::endl;
            disconnect();
            return false;
        }
        send_data(data.get_meta_package());
        for (int i = 0; i < data.get_chunck_count(); i++) {
            send_data(data.get_data_chunck(i));
        }
        return true;
    } else {
        std::cout << "Socket: Not connected" << std::endl;
        return false;
    }
}

bool Socket::send_data(const std::string& data)
{
    if (m_connected) {
        if (send(m_socket_fd, data.c_str(), data.length(), 0) == -1) {
            perror("send");
            disconnect();
            return false;
        }
        signal.on_packet_send.emit(this, data);
        return true;
    } else {
        std::cout << "Socket: Not connected" << std::endl;
        return false;
    }
}

std::string Socket::receive_data(int max_size, bool wait)
{
    if (m_connected) {
        ssize_t numbytes = 0;
        char* buf = (char*)malloc(sizeof(char) * max_size);
        if ((numbytes = recv(m_socket_fd, buf, max_size - 1, wait ? 0 : MSG_DONTWAIT)) == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                free(buf);
                return "";
            } else {
                perror("recv");
                disconnect();
                free(buf);
                return "";
            }
        }
        std::string data = "";
        if (numbytes > 0) {
            buf[numbytes] = '\0';
            data = std::string(buf);
            signal.on_packet_receive.emit(this, data);
        }
        free(buf);
        return data;
    } else {
        std::cout << "Socket: Not connected" << std::endl;
        return "";
    }
}

bool Socket::is_connected()
{
    return m_connected;
}

void Socket::disconnect()
{
    close(m_socket_fd);
    if (m_connected) {
        m_connected = false;
        signal.on_disconnect.emit(this);
    }
}

Socket::~Socket()
{
    disconnect();
}
} // namespace LouisNet
