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
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include "ReadLicensePlates.hpp"
#ifdef RPI
#include <cppgpio.hpp>
#endif

#define PORT 6969

#define MAXDATASIZE 100

int sock = 0;
struct sockaddr_in serv_addr;

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
    return atoi(buffer);
}

void set_led(bool colour)
{

}

int main()
{

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    std::cout << get_ticket("ABCD") << std::endl;

    /*
    while(true)
    {
        set_led(false);
        std::cin.get();
        std::string license_plate = read_plate(0, rectangle(0, 0, 560, 380));
        uint64_t num = get_ticket(license_plate);
        std::ofstream ticket("/media/sdcard/ticket");
        if(ticket)
        {
            ticket << num;
            set_led(true);
        }
        else
        {
            set_led(false);
        }
        std::cout << license_plate << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    */

    return 0;
}