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
#include "ReadLicensePlates.hpp"

// #include <cppgpio.hpp>

uint64_t get_ticket(std::string license)
{

}

void write_ticket(uint64_t num)
{

}

int main()
{
   /*  GPIO::DigitalIn button(17, GPIO::GPIO_PULL::UP);
    while(true)
    {
        button.triggered();
        if(button.get_state())
            std::cout << "pushed" << std::endl;
    } */
    std::cout << read_plate(0) << std::endl;
    return 0;
}