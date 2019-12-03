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

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string.h>

namespace LouisNet {
class Socket;
class Packet {
private:
    std::string m_data;

    int m_maxsize;

    int m_chunck_count = 0;
    int m_expected_chuncks = 0;
    int m_expected_data = 0;

public:
    Packet();
    explicit Packet(const std::string& data, int maxsize = 1024);

    std::string get_data_chunck(int index);

    int get_chunck_count();
    int get_maxsize();
    int get_data_size();

    std::string get_data();

    std::string get_meta_package();

    void set_meta_data(const std::string& meta);
    void set_data(const std::string& data);
    void set_data(Socket* sock);
    void set_max_size(int maxsize);

    ~Packet();
};
} // namespace LouisNet
