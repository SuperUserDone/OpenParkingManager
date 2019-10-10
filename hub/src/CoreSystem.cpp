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

#include "CoreSystem.hpp"
#include "Utils.hpp"

bool CoreSystem::init()
{
	return true;
}
parking_space_t CoreSystem::get_parking(uint64_t ticket_number)
{
	return m_parking_data[m_ticket_data[ticket_number]];
}
uint64_t CoreSystem::issue_ticket(std::string reg_number)
{
	uint8_t* bytes = gen_random_bytes(8);
	uint64_t ticket = *(uint64_t*) bytes;
	free(bytes);
	m_ticket_data.emplace(ticket, reg_number);
	return ticket;
}
void CoreSystem::set_parking(std::string reg_number, parking_space_t place)
{
	m_parking_data[reg_number] = place;
}
void CoreSystem::destroy_ticket(uint64_t ticket_number)
{
	m_parking_data.erase(m_ticket_data[ticket_number]);
	m_ticket_data.erase(ticket_number);
}
