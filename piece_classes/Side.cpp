//
// Created by luca on 3/18/23.
//

#include "Side.h"
#include "iostream"
#include "util_piece.h"
#include <algorithm>
#include <opencv2/opencv.hpp>
using namespace cv;

int Side::compare_res = 1200;

Side::Side(Mat& shape, Piece* piece_, int piece_side_, Point p1, Point p2, Point center){

    assert(shape.type() == CV_8U);

    piece = piece_;
    piece_side = piece_side_;
    border_shape = Mat::zeros(Side::compare_res, Side::compare_res, CV_8U);

    // Calculate the midpoint between p1 and p2
    float midX = (p1.x + p2.x) / 2.0f;
    float midY = (p1.y + p2.y) / 2.0f;

    // Calculate the angle between p1 and p2
    float angle = std::atan2(p2.y - p1.y, p2.x - p1.x) * 180.0f / CV_PI;

    // Create a transformation matrix to rotate and move the input image
    // docs: https://docs.opencv.org/3.4/da/d54/group__imgproc__transform.html#gafbbc470ce83812914a70abfb604f4326
    cv::Mat transform = cv::getRotationMatrix2D(cv::Point2f((p1+p2)/2), angle, 1.0);
    transform.at<double>(0, 2) += (float)Side::compare_res/2 - midX;
    transform.at<double>(1, 2) += (float)Side::compare_res/2 - midY;

    // Apply the transformation to the input image and put it in the output image
    warpAffine(shape, border_shape, transform, border_shape.size());

    // translating my coordinates to the new form
    std::vector<Point2f> points = std::vector<Point2f>(),  points_transformed = std::vector<Point2f>();
    points.push_back((Point2f)p1);
    points.push_back((Point2f)p2);
    points.push_back((Point2f)center);
    cv::transform(points,points_transformed,transform);
    p1 = points_transformed[0];
    p2 = points_transformed[1];
    center = points_transformed[2];

    // applying the threshold since the transformation has blurred the border
    border_shape = border_shape>127;

    //creating 2 lines from the 2 points to the border of the image
    line(border_shape,p1,p1+(p1-p2), Scalar(255),3);
    line(border_shape,p2,p2+(p2-p1), Scalar(255),3);

    // connecting the lines to the center to make it a connected piece
    line(border_shape,p1,p1+(center-p1)/5, Scalar(255),3);
    line(border_shape,p2,p2+(center-p2)/5, Scalar(255),3);

    // flood filling significant points to keep only one border
    floodFill(border_shape,Point(0,0),100);
    border_shape = border_shape != 100;


    int difference = Side::compare_res*Side::compare_res/2 - countNonZero(border_shape);


    //std::cout << difference << std::endl;

    if (difference > 15000){
        kind = SideKind::FEMALE;
        //std::cout << "female" << std::endl;
    }else if(difference < -15000){
        kind = SideKind::MALE;
        //std::cout << "male" << std::endl;
    }else{
        kind = SideKind::BORDER;
        //std::cout << "border" << std::endl;
    }

    /*
    Mat temp;
    resize(border_shape,temp,Size(500,500) );
    imshow("new_side", temp);
    waitKey(0);
    */

    // doto: add piece kind
}


#define EROSION_SIZE (6*Side::compare_res/1200)
float Side::compare_to(Side &other,bool debug) {

    // quick comp
    if(!debug){
        if(kind == other.kind){
            return 0;
        }
        if(kind == SideKind::BORDER || other.kind == SideKind::BORDER){
            return 0;
        }
    }

    Mat other_border_shape;
    rotate(other.border_shape, other_border_shape, ROTATE_180);
    Mat result;
    bitwise_xor(border_shape, other_border_shape,result);

    // dilate the result to consider only wide gaps areas and not tiny borders
    Mat kernel = Mat::zeros(Size(EROSION_SIZE,EROSION_SIZE),CV_8U) == 0;
    Mat result_expanded;
    dilate(result,result_expanded,kernel);

    float compatibility = ((float)countNonZero(result_expanded)) / (Side::compare_res*Side::compare_res);

    if(debug){
        std::string name = std::string("compatibility: ") + std::to_string(compatibility*100) + std::string("%");
        Mat temp;
        resize(result_expanded,temp,result_expanded.size()/4);
        imshow(name,temp);
        waitKey(0);
        destroyAllWindows();
    }

    return compatibility;

}

Side::Side() {
    border_shape = Mat();
}

SideKind Side::get_kind() {
    return kind;
}

Piece &Side::get_piece() {
    return *piece;
}

void Side::set_compare_res(int res) {
    compare_res = res;
}

int Side::get_compare_res() {
    return compare_res;
}
