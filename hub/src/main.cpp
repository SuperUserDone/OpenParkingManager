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

#include <iostream>

#include "LouisNet/LouisNet.hpp"

using namespace LouisNet;

static void connect_signals(Socket* sock)
{
    sock->signal.on_connect.connect([](Socket* s, const std::string& addr, const std::string& port, const int& fd) {
        std::cout << "Connected to " << addr << ":" << port << " with FD: " << fd << "!" << std::endl;
    });
    sock->signal.on_packet_send.connect([](Socket* s, const std::string& packet) {
        std::cout << "Sent: \n"
                  << packet << "\nto " << s->get_address() << ":" << s->get_port() << "!" << std::endl;
    });
    sock->signal.on_packet_receive.connect([](Socket* s, const std::string& packet) {
        std::cout << "Recieved:\n"
                  << packet << "\nfrom " << s->get_address() << ":" << s->get_port() << "!" << std::endl;
    });
    sock->signal.on_disconnect.connect([](Socket* s) {
        std::cout << "Socket disconnected!" << std::endl;
    });
}
int main(void)
{
    Listener lis("6969");

    lis.signal.on_accept.connect([](Listener* l, AsyncSocket* s) {
        connect_signals(s->get_socket());
        std::cout << "Got connection from " << s->get_socket()->get_address() << ":" << s->get_socket()->get_port() << std::endl;
    });

    lis.listen_async();

    while (true) {
        /* code */
    }

    return 0;
}
