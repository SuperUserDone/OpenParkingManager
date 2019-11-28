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

#include <networking/Socket.hpp>
#include <networking/Packet.hpp>

int main(void)
{
    Socket sock("127.0.0.1", "6969");
    sock.connect_socket();
    Packet data("ABCDEFGHIJK", 8);
    sock.send_data(data);
    std::cout << sock.receive_data() << std::endl;
    return 0;
}