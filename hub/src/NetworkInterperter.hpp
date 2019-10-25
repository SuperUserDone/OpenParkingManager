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

#pragma once

#include "IBaseSystem.hpp"
#include "Database.hpp"

class NetworkInterperter : public IBaseSystem
{
public:
    NetworkInterperter();
    NetworkInterperter(Database* system);

    //	Init functions
	void init() override;
	void start() override;

	//	I/O functions
	void write(uint8_t* data, int len) override;
	void write(const char* data, int len);
    void free_write_req(uv_write_t *req);
    void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    void on_close(uv_handle_t* handle);
    void echo_write(uv_write_t *req, int status);
    void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    void on_new_connection(uv_stream_t *server, int status);

    // System Functions
    uint64_t issue_ticket();

    // PacketParser
    std::string parse_packet(std::string packet);
private:
    //  System
    Database* m_system;
    //  Socket
    struct sockaddr_in addr;
    uv_tcp_t m_server;
    // Port
    const uint16_t SYS_PORT = 6969;
};
