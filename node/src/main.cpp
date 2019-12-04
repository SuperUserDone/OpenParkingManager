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

#include <json.hpp>

#include "ReadLicensePlates.hpp"

//#include <cppgpio.hpp>

#define PORT 6969

#define MAXDATASIZE 100

using json = nlohmann::json;

std::string ip = "";

int sock = 0;
struct sockaddr_in serv_addr;

std::vector<std::string> row_left = { "01A01", "01A02", "01A03", "01A04", "01A05", "01A06", "01A07" };
std::vector<std::string> row_right = { "01B01", "01B02", "01B03", "01B04", "01B05", "01B06", "01B07" };

int c_pos = 0;

//GPIO::DigitalIn Limit0(20, GPIO::GPIO_PULL::UP);
//GPIO::DigitalIn Limit1(16, GPIO::GPIO_PULL::UP);

bool triggered = false;

int increment = 1;
std::string send_and_recieve(std::string packet)
{
    char buffer[MAXDATASIZE] = { 0 };
    send(sock, packet.c_str(), packet.size(), 0);
    int numbytes;
    if ((numbytes = recv(sock, buffer, MAXDATASIZE - 1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buffer[numbytes] = '\0';
    return std::string(buffer);
}

int main()
{
    json config;

    std::ifstream config_file("node_config.json");
    config_file >> config;

    ip = config["config"]["ip"];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // while (true) {
    //     while (!Limit0.triggered()) {
    //         if (Limit1.triggered()) {
    //             triggered = !triggered;
    //             if (triggered) {
    //                 if (c_pos == 0) {
    //                     increment = 1;
    //                 }
    //                 if (c_pos == 6) {
    //                     increment = -1;
    //                 }
    //                 c_pos += increment;
    //                 std::cout << c_pos << std::endl;
    //             }
    //         }
    //     }
    //     while (!Limit0.triggered()) {
    //     }
    //     std::cout << "OK" << std::endl;
    //     std::string license_plate_left = read_plate(0, rectangle(0, 200, 640 - 1, 480 - 200));
    //     std::string license_plate_right = read_plate(2, rectangle(0, 200, 640 - 1, 480 - 200));
    //     std::string packet = "RP";
    //     packet.append(row_left[c_pos]);
    //     packet.append("\n");
    //     send_and_recieve(packet);
    //     packet = "RP";
    //     packet.append(row_right[c_pos]);
    //     packet.append("\n");
    //     send_and_recieve(packet);
    //     if (license_plate_left != "") {
    //         packet = "AP";
    //         packet.append(row_left[c_pos]);
    //         packet.append(license_plate_left);
    //         packet.append("\n");
    //         send_and_recieve(packet);
    //     }
    //     if (license_plate_right != "") {
    //         packet = "AP";
    //         packet.append(row_right[c_pos]);
    //         packet.append(license_plate_right);
    //         packet.append("\n");
    //         send_and_recieve(packet);
    //     }
    // }
    return 0;
}