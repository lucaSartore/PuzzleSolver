#include "util.h"

#include <opencv2/opencv.hpp>

/// get a mat that is black on the border, and make it printable by cutting it down
void crop_image_to_remove_black_gb(cv::Mat& inputImage)
{
    cv::Mat grayImage, binaryImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    cv::threshold(grayImage, binaryImage, 0, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    std::vector<cv::Point> total_points = {};

    for(auto v: contours){
        total_points.insert(total_points.end(),v.begin(),v.end());
    }

    auto rect = cv::boundingRect(total_points);

    const int marginSize = 50;

    int x1 = std::max(rect.x - marginSize, 0);
    int y1 = std::max(rect.y - marginSize, 0);
    int x2 = std::min(rect.x + rect.width + marginSize, inputImage.size().width-1);
    int y2 = std::min(rect.y + rect.height + marginSize, inputImage.size().height-1);

    cv::Rect roi(cv::Point(x1,y1),cv::Point(x2,y2));
    cv::Mat croppedImage = inputImage(roi);
    inputImage = croppedImage;
}