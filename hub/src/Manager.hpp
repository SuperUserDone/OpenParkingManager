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
#include <stdint.h>
#include <string>
#include <uv.h>

#include "CoreSystem.hpp"
#include "IBaseSystem.hpp"
#include "CommandLine.hpp"
#include "NetworkInterperter.hpp"

class Manager
{
public:
    Manager();
    bool init();
    int run();
	void close();
protected:
	NetworkInterperter m_communication_system;
    uv_loop_t* m_loop;
};
