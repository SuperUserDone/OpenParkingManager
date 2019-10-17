#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include <iostream>
#include <vector>

bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i < j );
}

std::string read_plate(int camera_number)
{
    cv::VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if(!cap.open(camera_number))
        return "";
    cv::Mat frame;
    cap >> frame;

    cv::Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);

    cv::Mat blur;
    cv::bilateralFilter(grey, blur, 11, 17, 17);

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

    //cv::drawContours(frame, std::vector<std::vector<cv::Point>>{license_plate},-1,cv::Scalar(255,0,0),3);

    cv::Mat mask(frame.size(), CV_8UC1);
    mask = 0;
    cv::fillPoly(mask, std::vector<std::vector<cv::Point>>{license_plate}, cv::Scalar(255), 8, 0);

    cv::Mat final_image;
    bitwise_and(frame, frame, final_image, mask);

    cv::RotatedRect licence_plate_rect = minAreaRect(license_plate);

    //cv::rectangle(final_image, licence_plate_rect.boundingRect(), cv::Scalar(255,0,0));

    cv::Mat cropped_image = final_image(licence_plate_rect.boundingRect());

    cv::cvtColor(cropped_image, cropped_image, cv::COLOR_BGR2GRAY);

    cv::threshold(cropped_image, cropped_image, 148, 255, cv::THRESH_BINARY);

    tesseract::TessBaseAPI tess;
    if (tess.Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    tess.SetImage((uchar*)cropped_image.data, cropped_image.size().width, cropped_image.size().height, cropped_image.channels(), cropped_image.step1());
    tess.Recognize(0);

    while(1)
    {
        if( cv::waitKey(10) == 27 ) break;
        cv::imshow("Test", cropped_image);
    }
    return tess.GetUTF8Text();
}
