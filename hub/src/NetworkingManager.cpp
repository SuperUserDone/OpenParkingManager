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

#include "NetworkingManager.hpp"
#include <ReadLicensePlates.hpp>
#include <base64.hpp>
#include <opencv2/opencv.hpp>
#include <sha1.hpp>

NetworkingManager::NetworkingManager()
    : m_listener("6969")
    , m_locate_ticket("LT.*\n?")
    , m_image("MP(\n.*)*")
{
    m_db = &Database::getInstance();
    m_locate_ticket.set_run_function(std::bind(&NetworkingManager::locate_ticket_callback, this, std::placeholders::_1, std::placeholders::_2));
    m_image.set_run_function(std::bind(&NetworkingManager::image_receive_callback, this, std::placeholders::_1, std::placeholders::_2));

    m_listener.listen_async();
}

void NetworkingManager::image_receive_callback(LouisNet::Socket* sock, const std::string& data)
{
    LouisNet::Packet temp;
    temp.set_meta_data(data);
    temp.set_data(sock);

    std::string fcontent = temp.get_data();

    std::string flags = sock->receive_data(1024, true, false);

    std::string final_str = base64_decode(fcontent);

    std::vector<char> img(final_str.begin(), final_str.end());

    std::string uid = sha1(fcontent);
    fcontent.clear();

    cv::Mat raw_data(img);

    std::stringstream flags_stream(flags);

    std::string flag_type, flag_data;
    flags_stream >> flag_type >> flag_data;
    std::cout << "FLAG: " << flag_type << std::endl;
    if (flag_type == "ENTRY") {
        std::string license_plate = read_plate(cv::imdecode(raw_data, true), cv::Rect(0, 0, 0, 0), "./data/img/", uid, "entry");
        std::cout << "License plate " << license_plate << std::endl;
        sock->send_data(uid);
        m_db->store_vehicle(license_plate, uid);
    }
    if (flag_type == "STORE") {
        std::string license_plate = read_plate(cv::imdecode(raw_data, true), cv::Rect(0, 0, 0, 0), "./data/img/", uid, "");
        std::string ticket = m_db->get_ticket_by_license(license_plate);

        try {
            std::filesystem::create_directories("./data/img/" + ticket + "/parked/" + uid + "");
            std::filesystem::rename("./data/img/" + uid, "./data/img/" + ticket + "/parked/" + uid);
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "License plate " << license_plate << std::endl;
        m_db->store_parking(license_plate, flag_data);
        sock->send_data("OK");
    }
}

void NetworkingManager::locate_ticket_callback(LouisNet::Socket* sock, const std::string& data)
{
    std::string data_args = data.substr(2);
    size_t location = data_args.find("\n");
    while (location != std::string::npos) {
        data_args.erase(location, 1);
        location = data_args.find("\n");
    }
    std::string result = m_db->get_parking_by_ticket(data_args);
    sock->send_data((result == "") ? "NULL" : result);
}

NetworkingManager::~NetworkingManager()
{
}