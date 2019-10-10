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

#include <iostream>
#include <string>
#include <stdint.h>
#include <map>

typedef struct
{
	uint8_t floor;
	uint8_t row;
	uint8_t parking;
} parking_space_t;

class CoreSystem
{
public:
	bool init();
	parking_space_t get_parking(uint64_t ticket_number);
	uint64_t issue_ticket(std::string reg_number);
	void set_parking(std::string regNumber, parking_space_t place);
	void destroy_ticket(uint64_t ticket_number);
private:
	std::map<std::string, parking_space_t> m_parking_data;
	std::map<uint64_t, std::string> m_ticket_data;
};
