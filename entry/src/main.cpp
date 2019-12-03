/*
OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include <json.hpp>

#define PORT 6969

#define MAXDATASIZE 100

using json = nlohmann::json;

std::string disk = "/run/media/louis/178B-7CAB";
std::string ip = "localhost";

int main()
{
    json config;

    std::ifstream config_file("entry_config.json");
    config_file >> config;

    ip = config["config"]["ip"];
    disk = config["config"]["disk"];

    std::ifstream in_file("cutecat.jpg", std::ios::binary | std::ios::app);

    return 0;
}