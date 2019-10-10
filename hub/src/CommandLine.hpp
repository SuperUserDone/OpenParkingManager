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
#include <uv.h>

#include "CoreSystem.hpp"
#include "IBaseSystem.hpp"
#include "CommandLine_uvWrap.hpp"

class CommandLine : public IBaseSystem
{
public:
	CommandLine();
	CommandLine(CoreSystem* system);
	//	Init functions
	void init() override;
	void start() override;

	//	I/O functions
	void write(uint8_t* data, int len) override;
	void write(const char* data, int len);
	void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	void free_write_req(uv_write_t *req);
	void on_stdout_write(uv_write_t *req, int status);
	void on_file_write(uv_write_t *req, int status);
	void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb);
	void read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

	//	System related
	void set_system_ptr(CoreSystem* system);
	CoreSystem* get_system_ptr();
private:
	// 	Processing
	void process_text(std::string text);
	//	Text
	void print_help();
	void print_license();
	void print_license_brief();
	void print_license_nw();
	void print_license_terms();
	//	I/O
	uv_pipe_t stdout_pipe;
	uv_pipe_t stdin_pipe;
	//	System
	CoreSystem* m_system;
};
