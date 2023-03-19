//
// Created by luca on 3/18/23.
//

#include "Side.h"
#include "iostream"
#include "util_piece.h"
#include <opencv2/opencv.hpp>
using namespace cv;

Side::Side(Mat& shape, Piece* piece_, int piece_side_, Point p1, Point p2){
    piece = piece_;
    piece_side = piece_side_;
    border_shape = Mat::zeros(Piece::get_compare_res(), COMPARE_RES, CV_8U);
    border_mask = Mat();
    // todo_ rotate teh mat and create the border mask and define border kind

    // Calculate the midpoint between p1 and p2
    float midX = (p1.x + p2.x) / 2.0f;
    float midY = (p1.y + p2.y) / 2.0f;

    // Calculate the angle between p1 and p2
    float angle = std::atan2(p2.y - p1.y, p2.x - p1.x) * 180.0f / CV_PI;

    // Create a transformation matrix to rotate and move the input image
    cv::Mat transform = cv::getRotationMatrix2D(cv::Point2f((p1+p2)/2), angle, 1.0);
    transform.at<double>(0, 2) += COMPARE_RES/2 - midX;
    transform.at<double>(1, 2) += COMPARE_RES/2 - midY;

    // Apply the transformation to the input image and put it in the output image
    cv::warpAffine(shape, border_shape, transform, border_shape.size());

    Mat temp;
    resize(border_shape,temp,Size(500,500) );
    imshow("new_side", temp);
    waitKey(0);

}

float Side::compare_to(Side &other,bool debug) {
    return 0;
}

Side::Side() {
    border_shape = Mat();
    border_mask = Mat();
}

SideKind Side::get_kind() {
    return kind;
}

Piece &Side::get_piece() {
    return *piece;
}
