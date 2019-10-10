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
#include "Manager.hpp"

Manager::Manager()
{
	m_loop = uv_default_loop();
}

bool Manager::init()
{
	m_communication_system.init();
	m_communication_system.start();
	return true;
}

void Manager::close()
{
	uv_loop_close(m_loop);
}

int Manager::run()
{
	uv_run(m_loop, UV_RUN_DEFAULT);
	return 0;
}
