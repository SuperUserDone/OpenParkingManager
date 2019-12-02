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

#include <networking/Packet.hpp>
#include <networking/Socket.hpp>
#include <networking/AsyncSocket.hpp>

int main(void)
{
    Socket sock("127.0.0.1", "6969");
    sock.signal.on_connect.connect([](Socket* s, const std::string& addr, const std::string& port, const int& fd) {
        std::cout << "Connected to " << addr << ":" << port << " with FD: " << fd << "!" << std::endl;
    });
    sock.signal.on_packet_send.connect([](Socket* s, const std::string& packet){
        std::cout << "Sent: \n" << packet << "\nto " << s->get_address() << ":" << s->get_port() << "!" << std::endl;
    });
    sock.signal.on_packet_receive.connect([](Socket* s, const std::string& packet){
        std::cout << "Recieved:\n" << packet << "\nfrom " << s->get_address() << ":" << s->get_port() << "!" << std::endl;
    });
    sock.signal.on_disconnect.connect([](Socket* s){
        std::cout << "Socket disconnected!" << std::endl;
    });
    sock.connect_socket();
    AsyncSocket asock(&sock);
    asock.start();
    Packet data("ABCDEFGHIJK", 16);
    asock.getSocket()->check_connected();
    asock.getSocket()->send_data(data);
    asock.kill();
    return 0;
}