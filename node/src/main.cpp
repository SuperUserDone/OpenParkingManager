/*
OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <fstream>
#include <iostream>
#include <string>

#include <base64.hpp>
#include <json.hpp>

#include <LouisNet.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;

std::vector<std::string> left = { "1A1", "1A2", "1A3", "1A4", "1A5", "1A6", "1A7" };
std::vector<std::string> right = { "1B1", "1B2", "1B3", "1B4", "1B5", "1B6", "1B7" };

int cameras[] = {0, 1};

int main()
{
    json config;

    std::ifstream config_file("node_config.json");
    config_file >> config;
    config_file.close();

    std::string ip = config["config"]["ip"];

    while (true) {
        int place;
        std::cin >> place;
        place--;
        for (int i = 0; i < 2; i++) {
            cv::Mat frame;
            try {
                cv::VideoCapture cam(cameras[i]);
                cam >> frame;
            } catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
            }

            if (!frame.empty()) {
                std::vector<uchar> buffer;
                buffer.resize(1024 * 1024 * 10);
                cv::imencode(".jpg", frame, buffer);
                buffer.shrink_to_fit();
                std::string str(buffer.begin(), buffer.end());
                buffer.clear();

                std::string b64str = base64_encode((const unsigned char*)str.c_str(), str.length());

                LouisNet::Packet pack(b64str, 128);
                LouisNet::Socket sock(ip, "6969");
                sock.m_maxsize = 128;
                sock.connect_socket();
                sock.send_data(pack);

                std::cout << sock.receive_data() << std::endl;
                i ? sock.send_data("STORE " + left[place]) : sock.send_data("STORE " + right[place]);
            }
        }
    }

    return 0;
}