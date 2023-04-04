//
// Created by luca on 3/18/23.
//

// define the percentage of discrepancy that there could be for a piece to another in order fot them to be considered a mathc
#define MAX_SIDE_LENGHT_ERROR 0.1
// side of the kernel that will be use to increase the thickness of the border
#include "Side.h"
#include "iostream"
#include "util_piece.h"
#include "math.h"
#include "Piece.h"

#include <algorithm>
#include <opencv2/opencv.hpp>
using namespace cv;

int Side::compare_res = 1200;


#define BORDER_THICKNESS (22*Side::compare_res/1200)
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

    side_lenght = p2.x-p1.x;

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

    // calculating making the kenny filter to the border
    Mat canny_border;
    Canny(border_shape,canny_border,50,200);

    // eroding the border for better comparison
    Mat kernel = Mat::ones(Size(BORDER_THICKNESS, BORDER_THICKNESS),CV_8U) == 1;
    dilate(canny_border,border_shape,kernel);

    /*
    Mat temp;
    resize(border_shape,temp,Size(500,500) );
    imshow("new_side", temp);
    waitKey(0);
     */
}


float Side::compare_to(const Side &other,bool debug)const {

    // quick comp
    if(!debug) {
        if (kind == other.kind) {
            return 0;
        }
        if (kind == SideKind::BORDER || other.kind == SideKind::BORDER) {
            return 0;
        }
        /*
        float lenght_proportion = (float) side_lenght / (float) other.side_lenght;
        if (lenght_proportion > 1.0 + MAX_SIDE_LENGHT_ERROR) {
            return 0;
        }
        if (lenght_proportion < 1.0 / (1.0 + MAX_SIDE_LENGHT_ERROR)) {
            return 0;
        }*/
    }

    Mat other_border_shape_rotated;
    rotate(other.border_shape, other_border_shape_rotated, ROTATE_180);
    Mat and_mask, or_mask;
    bitwise_and(border_shape, other_border_shape_rotated, and_mask);
    bitwise_or(border_shape, other_border_shape_rotated, or_mask);

    float compatibility = ((float)countNonZero(and_mask)) / (float)countNonZero(or_mask);

    if(debug){
        std::string name = std::string("compatibility: ") + std::to_string(compatibility*100) + std::string("%");
        // create the colored image
        std::vector<cv::Mat> channels;
        channels.push_back(cv::Mat::zeros(and_mask.size(), CV_8U));
        channels.push_back(cv::Mat::zeros(and_mask.size(), CV_8U));
        channels.push_back(cv::Mat::zeros(and_mask.size(), CV_8U));

        border_shape.copyTo(channels[2]); // set red channel where border_shape is white
        other_border_shape_rotated.copyTo(channels[0]); // set blue channel where other_border_shape_rotated is white

        cv::Mat output_image;
        cv::merge(channels, output_image);

        Mat resized;
        resize(output_image,resized,output_image.size()/4);
        imshow(name,resized);
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
