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
#include "PacketListener.hpp"

namespace LouisNet {
PacketListener::PacketListener(/* args */)
{
    Socket::on_packet_receive.connect(std::bind(&PacketListener::handler, this, std::placeholders::_1, std::placeholders::_2));
}

PacketListener::PacketListener(const std::string& regex)
{
    set_regex(regex);
    Socket::on_packet_receive.connect(std::bind(&PacketListener::handler, this, std::placeholders::_1, std::placeholders::_2));
}

void PacketListener::handler(Socket* sock, const std::string& data)
{
    if (std::regex_match(data, std::regex(m_regex_string))) {
        m_run(sock, data);
    }
}

void PacketListener::set_regex(const std::string& regex)
{
    m_regex_string = regex;
}

void PacketListener::set_run_function(std::function<void(Socket* sock, const std::string& data)> func)
{
    m_run = func;
}
} // namespace LouisNet
