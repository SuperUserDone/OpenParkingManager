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

#include <arpa/inet.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include <base64.hpp>
#include <json.hpp>

#include <LouisNet.hpp>

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

    std::ifstream in_file("./images/group_photo.jpg");

    std::string str((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

    in_file.close();

    std::string b64str = base64_encode((const unsigned char*)str.c_str(), str.length());

    LouisNet::Packet pack(b64str, 128);
    LouisNet::Socket sock(ip, "6969");
    sock.m_maxsize = 128;
    sock.connect_socket();
    sock.send_data(pack);

    std::cout << pack.get_meta_package() << std::endl;

    for (int i = 0; i < pack.get_chunck_count(); i++) {
          std::cout << pack.get_data_chunck(i);
    }

    std::cout << std::endl;

    return 0;
}