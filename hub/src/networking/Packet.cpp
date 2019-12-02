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
#include <networking/Packet.hpp>

namespace LouisNet {
Packet::Packet(/* args */)
{
}

Packet::Packet(const std::string& data, int maxsize)
    : m_maxsize(maxsize)
    , m_data(data)
{
}

int Packet::get_data_size()
{
    return m_data.length();
}

int Packet::get_chunck_count()
{
    bool offset = 0;
    if (get_data_size() % m_maxsize > 0)
        offset = 1;
    return (get_data_size() / m_maxsize) + offset;
}

int Packet::get_maxsize()
{
    return m_maxsize;
}

std::string Packet::get_meta_package()
{
    std::stringstream package;
    package << "MP\n"
            << std::to_string(m_maxsize) << "\n"
            << get_chunck_count() << "\n"
            << get_data_size() << "\n\0";
    return package.str();
}

std::string Packet::get_data_chunck(int index)
{
    if (get_chunck_count() >= index) {
        std::string chunck = "";

        int chunck_start = index * m_maxsize;

        chunck = m_data.substr(chunck_start, m_maxsize);

        return chunck;
    } else {
        return "\n";
    }
}

void Packet::set_data(const std::string& data)
{
    m_data = data;
}

void Packet::set_max_size(int maxsize)
{
    m_maxsize = maxsize;
}

Packet::~Packet()
{
}
} // namespace LouisNet