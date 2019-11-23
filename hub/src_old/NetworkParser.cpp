/*
    OpenParkingManager- An open source parking manager and parking finder.
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
#include "NetworkInterperter.hpp"
#include <iostream>
#include <fstream>

std::string NetworkInterperter::parse_packet(std::string packet)
{
    std::cout << packet;
    std::string optcode(packet.begin(), packet.begin()+2);
    std::cout << optcode << std::endl;
    packet.erase(packet.begin(), packet.begin()+2);
    packet.erase(packet.end()-1, packet.end());
    std::cout << packet << std::endl;;
    switch (hash(optcode.c_str()))
    {
    case hash("IT"): // Issue ticket
    {
        uint64_t num = issue_ticket();
        m_system->store_vehicle(packet, num);
        return std::to_string(num);
    }
    break;
    case hash("PP"): // Put Parking into database
    {
        m_system->store_parking(std::string(packet.begin()+5, packet.end()), std::string(packet.begin(), packet.begin()+5));
        return "OK";
    }
    break;
    case hash("LT"): // Locate ticket
    {
        return m_system->get_parking_by_ticket(atoi_64(packet.c_str()));
    }
    case hash("RP"):
    {
        m_system->remove_parking(packet);
        return "OK";
    }
    break;
    default:
        return "ER";
        break;
    }
}