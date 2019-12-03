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

#include "LouisNet.hpp"

#include <regex>

namespace LouisNet {
class PacketListener
{
protected:
    std::function<void(Socket* sock, const std::string &data)> m_run;

    std::string m_regex_string;
public:
    PacketListener();
    PacketListener(const std::string &regex);

    void handler(Socket* sock, const std::string &data);
    void set_regex(const std::string &regex);

    void set_run_function(std::function<void(Socket* sock, const std::string &data)> func);
};
} // namespace LouisNet
