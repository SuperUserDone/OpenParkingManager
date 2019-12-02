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
#pragma once 

#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <string.h>
#include <iostream>
#include <sstream>

namespace LouisNet {
class Packet
{
private:
    std::string m_data;

    int m_maxsize;

    int m_chunck_count = 0;
public:
    Packet();
    explicit Packet(const std::string &data, int maxsize = 1024);

    std::string get_data_chunck(int index);

    int get_chunck_count();
    int get_maxsize();
    int get_data_size();

    std::string get_meta_package();

    void set_data(const std::string &data);
    void set_max_size(int maxsize);

    ~Packet();
};
} // namespace LouisNet
