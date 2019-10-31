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

#include <uv.h>
#include <ctime>
#include <sstream>
#include <random>

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

inline uint64_t atoi_64(const char* str)
{
    uint64_t val;
    std::istringstream ss(str);
    if (!(ss >> val))
        std::cout << "failed" << std::endl;
    return val;
}

inline uint8_t* gen_random_bytes(int bytes_count)
{
	srand(time(nullptr)+std::clock());
	uint8_t* bytes;
	bytes = (uint8_t*) malloc(sizeof(uint8_t) * bytes_count);
	for(int i = 0; i < bytes_count; i++)
	{
		bytes[i] = rand() % 0xFF;
	}
	return bytes;
}

constexpr unsigned int hash(const char *s, int off = 0) {
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];
}
