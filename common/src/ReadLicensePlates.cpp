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
#include "ReadLicensePlates.hpp"
#include <filesystem>

static std::string valid_chars = "BCDFGHIJKLMNOPQRSTVWXYZ123456789\0";

static bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2)
{
    double i = fabs(contourArea(cv::Mat(contour1)));
    double j = fabs(contourArea(cv::Mat(contour2)));
    return (i < j);
}

static std::string sanatise(std::string text)
{
    std::string final = "";
    for (char ti : text) {
        for (char it : valid_chars) {
            if (it == ti) {
                final.push_back(it);
            }
        }
    }
    if (final.length() < 7)
        final = "";
    return final;
}

std::string read_plate(cv::Mat frame, cv::Rect crop, const std::string& path, const std::string& UID, const std::string& context)
{
    std::string path_final = path + UID + "/" + context + "/";
    std::filesystem::create_directories(path_final);
    cv::imwrite(path_final + "out_frame.png", frame);

    if (crop.height == 0) {
        crop.height == frame.size().height;
    }
    if (crop.width == 0) {
        crop.width == frame.size().height;
    }

    //frame = frame(crop);

    cv::imwrite(path_final + "out_frame_cropped.png", frame);
    cv::Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    cv::imwrite(path_final + "out_grey.png", grey);

    cv::Mat blur;
    cv::bilateralFilter(grey, blur, 14, 17, 17);
    cv::imwrite(path_final + "out_blur.png", blur);

    cv::Mat edges;
    cv::Canny(blur, edges, 30, 200);
    cv::imwrite(path_final + "out_edges.png", edges);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    if (!contours.size())
        return "";

    std::cout << contours.size() << std::endl;
    ;

    std::sort(contours.begin(), contours.end(), compareContourAreas);

    std::reverse(contours.begin(), contours.end());

    std::vector<cv::Point> license_plate;

    for (std::vector<cv::Point> vec : contours) {
        if (fabs(cv::contourArea(vec)) > 20000) {
            double perimiter = cv::arcLength(vec, true);
            std::vector<cv::Point> approx_poligon;
            cv::approxPolyDP(vec, approx_poligon, 0.018 * perimiter, true);
            if (approx_poligon.size() == 4) {
                license_plate = approx_poligon;
                std::cout << "Found!!!" << std::endl;
                break;
            }
        }
    }

    if (!license_plate.size()) {
        if (contours[0].size() > 2)
            license_plate = contours[0];
        else
            return "";
    }

    cv::drawContours(frame, std::vector<std::vector<cv::Point>> { license_plate }, -1, cv::Scalar(255, 255, 255), 5);

    cv::Mat mask(frame.size(), CV_8UC1);
    mask = 0;
    cv::fillPoly(mask, std::vector<std::vector<cv::Point>> { license_plate }, cv::Scalar(255), 8, 0);
    cv::imwrite(path_final + "out_mask.png", mask);

    cv::Mat final_image;
    cv::bitwise_and(frame, frame, final_image, mask);

    cv::cvtColor(final_image, final_image, cv::COLOR_BGR2GRAY);
    cv::threshold(final_image, final_image, 148, 255, cv::THRESH_BINARY);

    cv::bitwise_not(mask, mask);
    cv::bitwise_or(final_image, mask, final_image, mask);
    cv::bitwise_not(final_image, final_image);

    cv::imwrite(path_final + "final_image.png", final_image);
    cv::RotatedRect crop_final = cv::minAreaRect(license_plate);

    cv::Mat cropped_image = final_image(crop_final.boundingRect());
    cv::imwrite(path_final + "final_image_cropped.png", cropped_image);

    tesseract::TessBaseAPI tess;
    if (tess.Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    tess.SetImage((uchar*)cropped_image.data, cropped_image.size().width, cropped_image.size().height, cropped_image.channels(), cropped_image.step1());
    tess.Recognize(0);

    /*
    while(1)
    {
        if( cv::waitKey(10) == 27 ) break;
        cv::imshow("Img", final_image);
    }*/

    std::string license_plate_text = tess.GetUTF8Text();
    license_plate_text = sanatise(license_plate_text);

    return license_plate_text;
}
