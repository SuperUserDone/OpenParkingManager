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

#include "NetworkInterperter.hpp"
#include "NetworkInterperter_uvWrap.hpp"

using namespace std::placeholders;

NetworkInterperter::NetworkInterperter()
{
    if(m_system == nullptr)
        m_system = new Database();
}

NetworkInterperter::NetworkInterperter(Database* system)
{
    m_system = system;
}

void NetworkInterperter::init()
{
    on_new_connection_cpp = std::bind(&NetworkInterperter::on_new_connection, this, _1, _2);
    on_close_cpp = std::bind(&NetworkInterperter::on_close, this, _1);
    alloc_buffer_cpp = std::bind(&NetworkInterperter::alloc_buffer, this, _1, _2, _3);
    echo_read_cpp = std::bind(&NetworkInterperter::echo_read, this, _1, _2, _3);
    echo_write_cpp = std::bind(&NetworkInterperter::echo_write, this, _1, _2);
    
    m_loop = uv_default_loop();

    uv_tcp_init(m_loop, &m_server);

    uv_ip4_addr("0.0.0.0", SYS_PORT, &addr);

    uv_tcp_bind(&m_server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &m_server, 128, on_new_connection_uv);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return;
    }
}

void NetworkInterperter::start()
{
}

void NetworkInterperter::write(uint8_t* data, int len)
{
}

void NetworkInterperter::write(const char* data, int len)
{
}

void NetworkInterperter::free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void NetworkInterperter::alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void NetworkInterperter::on_close(uv_handle_t* handle) {
    free(handle);
}

void NetworkInterperter::echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
}

void NetworkInterperter::echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        //uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write_uv);
        parse_packet(std::string(req->buf.base));
        echo_write((uv_write_t*) req, 0);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, on_close_uv);
    }

    free(buf->base);
}

void NetworkInterperter::on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(m_loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer_uv, echo_read_uv);
    }
    else {
        uv_close((uv_handle_t*) client, on_close_uv);
    }
}

