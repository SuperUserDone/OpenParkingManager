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

static void connect_signals()
{
    Socket::on_connect.connect([](Socket* s, const std::string& addr, const std::string& port, const int& fd) {
        std::cout << "\33[32mConnected to " << addr << ":" << port << " with FD: " << fd << "!\33[0m" << std::endl;
    });
    Socket::on_packet_send.connect([](Socket* s, const std::string& packet) {
        std::cout << "Sent: \n"
                  << packet << "\nto " << s->get_address() << ":" << s->get_port() << "!" << std::endl;
    });
    Socket::on_packet_receive.connect([](Socket* s, const std::string& packet) {
        std::cout << "Recieved:\n"
                  << packet << "\nfrom " << s->get_address() << ":" << s->get_port() << "!" << std::endl;
    });
    Socket::on_disconnect.connect([](Socket* s) {
        std::cout << "Socket disconnected!" << std::endl;
    });
}

static void test(Socket* s, const std::string& packet)
{
    std::cout << "got large_packet_listener!" << std::endl;
}

int main(void)
{
    PacketListener large_packet_listener("MP\n\\d+\n\\d+\n\\d+\n");
    PacketListener large_packet_listener2("OK");
    large_packet_listener2.set_run_function(test);
    large_packet_listener.set_run_function(test);
    connect_signals();
    Listener lis("6969");

    Listener::on_accept.connect([](Listener* l, AsyncSocket* s) {
        std::cout << "\33[34mGot connection from " << s->get_socket()->get_address() << ":" << s->get_socket()->get_port() << "\33[0m" << std::endl;
        Packet test_packet("000000000000000000000", 16);
        s->get_socket()->send_data(test_packet);
    });

    lis.listen_async();

    while (true) {
        /* code */
    }

    return 0;
}
