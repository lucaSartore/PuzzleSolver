//
// Created by luca on 5/1/23.
//

#ifndef PIECECLASS_UTIL_H
#define PIECECLASS_UTIL_H

#endif //PIECECLASS_UTIL_H

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

    cv::RotatedRect rect = cv::minAreaRect(total_points);

    int marginSize = std::min(std::min(rect.center.x, rect.center.y),
                              std::min(inputImage.cols - rect.center.x, inputImage.rows - rect.center.y));
    marginSize = std::min(marginSize, 50);
    int x = std::max(static_cast<int>(rect.center.x - rect.size.width / 2 - marginSize), 0);
    int y = std::max(static_cast<int>(rect.center.y - rect.size.height / 2 - marginSize), 0);
    int width = std::min(static_cast<int>(rect.size.width + 2 * marginSize), inputImage.cols - x);
    int height = std::min(static_cast<int>(rect.size.height + 2 * marginSize), inputImage.rows - y);


    cv::Rect roi(x, y, width, height);
    cv::Mat croppedImage = inputImage(roi);
    inputImage = croppedImage;
}