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
//	This file is a hack to make libuv's callbacks work in refrence of an object

#pragma once

#include <uv.h>
#include <functional>

inline std::function<void(uv_stream_t*, int)> on_new_connection_cpp;
inline std::function<void(uv_handle_t*)> on_close_cpp;
inline std::function<void(uv_handle_t*, size_t, uv_buf_t*)> alloc_buffer_cpp;
inline std::function<void(uv_stream_t*, ssize_t, const uv_buf_t*)> echo_read_cpp;
inline std::function<void(uv_write_t/*
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
*/*, int)> echo_write_cpp;

inline void on_new_connection_uv(uv_stream_t *server, int status)
{
    on_new_connection_cpp(server, status);
}

inline void on_close_uv(uv_handle_t* handle)
{
    on_close_cpp(handle);
}

inline void alloc_buffer_uv(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    alloc_buffer_cpp(handle, suggested_size, buf);
}

inline void echo_read_uv(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    echo_read_cpp(client, nread, buf);
}

inline void echo_write_uv(uv_write_t *req, int status)
{
    echo_write_cpp(req, status);
}