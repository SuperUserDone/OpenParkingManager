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

#include "ReadLicensePlates.hpp"

#ifdef RPI
#include <cppgpio.hpp>
#endif

#define PORT 6969

#define MAXDATASIZE 100

using json = nlohmann::json;

std::string disk = "/run/media/louis/178B-7CAB";
std::string ip = "localhost";

int sock = 0;
struct sockaddr_in serv_addr;

uint64_t atoi_64(const char* str)
{
    uint64_t val;
    std::istringstream ss(str);
    if (!(ss >> val))
        std::cout << "failed" << std::endl;
    return val;
}

uint64_t get_ticket(std::string license)
{
    char buffer[MAXDATASIZE] = {0};
    std::string command = "IT";
    command.append(license);
    command.append("\n");
    send(sock, command.c_str(), command.size(), 0 );
    int numbytes;
    if ((numbytes = recv(sock, buffer, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buffer[numbytes] = '\0';
    std::cout << buffer;
    return atoi_64(buffer);
}

void set_led(bool colour)
{

}

int main()
{
    json config;

    std::ifstream config_file("entry_config.json");
    config_file >> config;

    ip = config["config"]["ip"];
    disk = config["config"]["disk"];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(true)
    {
        set_led(false);
        std::cin.get();
        std::string license_plate = read_plate(0, rectangle(0, 0, 580, 340));
        if(license_plate == "")
        {
            for(int i = 0; i < 5; i++)
            {
                set_led(false);
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                set_led(true);
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        }
        else
        {
            uint64_t num = get_ticket(license_plate);
            std::string data_location = disk;
            data_location.append("/ticket.txt");
            std::ofstream ticket(data_location);
            if(ticket)
            {
                ticket << num;
                set_led(true);
            }
            else
            {
                set_led(false);
            }
            ticket.close();
            std::cout << license_plate << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
    return 0;
}