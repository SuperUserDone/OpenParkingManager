/*
    OpenParkingManager- An open source parking manager and parking finder.
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
#include <uv.h>
#include <functional>
#include <iostream>

#include "CommandLine.hpp"
#include "Database.hpp"

using namespace std::placeholders;

CommandLine::CommandLine()
{
    CommandLine(new Database());
}

CommandLine::CommandLine(Database* system)
{
    m_system = system;
}

void CommandLine::init()
{
	alloc_buffer_cpp = std::bind(&CommandLine::alloc_buffer, this, _1, _2, _3);
	read_stdin_cpp = std::bind(&CommandLine::read_stdin, this, _1, _2, _3);
	on_stdout_write_cpp = std::bind(&CommandLine::on_stdout_write, this, _1, _2);
	m_loop = uv_default_loop();

	uv_pipe_init(m_loop, &stdin_pipe, 0);
    uv_pipe_open(&stdin_pipe, 0);

    uv_pipe_init(m_loop, &stdout_pipe, 0);
    uv_pipe_open(&stdout_pipe, 1);

	print_license_brief();

	uv_read_start((uv_stream_t*)&stdin_pipe, alloc_buffer_uv, read_stdin_uv);
	write(">", 1);
}

void CommandLine::start()
{

}

void CommandLine::write(const char* data, int len)
{
	uv_buf_t buf;
    buf = uv_buf_init((char*) malloc(len), len);
	strcpy(buf.base, data);
	write_data((uv_stream_t *)&stdout_pipe, len, buf, on_stdout_write_uv);
	if (buf.base)
        free(buf.base);
}

void CommandLine::write(uint8_t* data, int len)
{
	write((char*) data, len);
}

void CommandLine::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    *buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

void CommandLine::free_write_req(uv_write_t *req)
{
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void CommandLine::on_stdout_write(uv_write_t *req, int status)
{
    free_write_req(req);
}

void CommandLine::write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb)
{
    write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
    req->buf = uv_buf_init((char*) malloc(size), size);
    memcpy(req->buf.base, buf.base, size);
    uv_write((uv_write_t*) req, (uv_stream_t*)dest, &req->buf, 1, cb);
}

void CommandLine::read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    if (nread < 0)
	{
        if (nread == UV_EOF)
		{
            // end of file
            uv_close((uv_handle_t *)&stdin_pipe, NULL);
            uv_close((uv_handle_t *)&stdout_pipe, NULL);
        }
    }
	else if (nread > 0)
	{
        process_text(std::string(buf->base, buf->len));
    }

    if (buf->base)
        free(buf->base);
	write(">", 1);
}

void CommandLine::set_system_ptr(Database* system)
{
	m_system = system;
}

Database* CommandLine::get_system_ptr()
{
	return m_system;
}
