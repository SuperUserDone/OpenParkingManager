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

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include "ReadLicensePlates.hpp"

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i < j );
}

std::string read_plate(int camera_number, rectangle crop)
{
    cv::VideoCapture cap;
    if(!cap.open(camera_number))
        return "";
    cv::Mat frame;
    cap >> frame;

    cv::Rect ROI;
    ROI.x = crop.x;
    ROI.y = crop.y;
    ROI.width = frame.size().width-1;
    ROI.width = frame.size().height-1;
    if(crop.width <= frame.size().width)
        ROI.width = crop.width;
    if(crop.height <= frame.size().height)
        ROI.height = crop.height;
    //frame = frame(ROI);

    cv::Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);

    cv::Mat blur;
    cv::bilateralFilter(grey, blur, 14, 17, 17);

    cv::Mat edges;
    cv::Canny(blur, edges, 30, 200);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::sort(contours.begin(), contours.end(), compareContourAreas);

    std::reverse(contours.begin(), contours.end());

    std::vector<cv::Point> license_plate;

    for(std::vector<cv::Point> vec : contours)
    {
        double perimiter = cv::arcLength(vec, true);
        std::vector<cv::Point> approx_poligon;
        cv::approxPolyDP(vec, approx_poligon, 0.018 * perimiter, true);
        if(approx_poligon.size() == 4)
        {
            license_plate = approx_poligon;
            std::cout << "Found!!!" << std::endl;
            break;
        }
    }

    //cv::drawContours(frame, std::vector<std::vector<cv::Point>>{license_plate}, -1, cv::Scalar(255,255,0),5);

    cv::Mat mask(frame.size(), CV_8UC1);
    mask = 0;
    cv::fillPoly(mask, std::vector<std::vector<cv::Point>>{license_plate}, cv::Scalar(255), 8, 0);

    cv::Mat final_image;
    cv::bitwise_and(frame, frame, final_image, mask);
    cv::bitwise_not(mask, mask);
    cv::bitwise_or(cv::Scalar(255, 255, 255), final_image, final_image, mask);

    cv::RotatedRect license_plate_rect = minAreaRect(license_plate);

    //cv::rectangle(final_image, license_plate_rect.boundingRect(), cv::Scalar(255,0,0));

    cv::Mat rot_mat = cv::getRotationMatrix2D(license_plate_rect.center, license_plate_rect.angle, 1);

    cv::warpAffine(final_image, final_image, rot_mat, final_image.size(), cv::INTER_CUBIC);
    
    cv::Size box_size = license_plate_rect.size;
    cv::Mat cropped_image;
    cv::getRectSubPix(final_image, box_size, license_plate_rect.center, cropped_image);

    cv::cvtColor(cropped_image, cropped_image, cv::COLOR_BGR2GRAY);

    cv::threshold(cropped_image, cropped_image, 148, 255, cv::THRESH_BINARY);

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
        cv::imshow("Test", cropped_image);
    }
    */
    std::string license_plate_text = tess.GetUTF8Text();
    size_t pos = license_plate_text.find(' ');
    while(pos != std::string::npos)
    {
        license_plate_text.erase(pos, 1);
        pos = license_plate_text.find(' ');
    }

    return license_plate_text;
}
