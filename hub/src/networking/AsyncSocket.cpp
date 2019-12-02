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
#include "networking/AsyncSocket.hpp"

AsyncSocket::AsyncSocket(/* args */)
{
}
AsyncSocket::AsyncSocket(Socket* sock)
{
    m_sock = sock;
}

Socket* AsyncSocket::get_socket()
{
    return m_sock;
}
void AsyncSocket::setSocket(Socket* sock)
{
    m_sock = sock;
}

void AsyncSocket::receive_async()
{
    while (m_sock->is_connected()) {
        m_sock->receive_data(1024, false);
    }
}

void AsyncSocket::start()
{
    if (m_sock->is_connected())
        m_recive_async_thread = std::thread(std::bind(&AsyncSocket::receive_async, this));
    else
        std::cout << "Invalid socket" << std::endl;
}

void AsyncSocket::kill()
{
    if (m_sock != nullptr) {
        if (m_sock->is_connected())
            m_sock->disconnect();
        m_recive_async_thread.join();
        m_recive_async_thread.detach();
    }
}

AsyncSocket::~AsyncSocket()
{
    kill();
}
