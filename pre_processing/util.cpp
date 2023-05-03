//
// Created by luca on 3/16/23.
//

#include "util.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

/// show an image and resize ti... ti just save a few lines of code
void show(Mat &m){
    Mat temp;
    resize(m,temp,Size(400,400));
    imshow("",temp);
    waitKey(0);
}

/// find the convex hull of a mask, and fill it with white.
void quick_convex_hull(Mat &input, Mat &output){

    // create output_
    output =  Mat::zeros(input.size(), CV_8U );

    Mat canny;
    Canny(input, canny, 50, 200 );

    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );


    vector<vector<Point>>hull( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );
    }

    for( size_t i = 0; i< contours.size(); i++ )
    {
        drawContours(output, hull, (int)i, 255 );
    }

    floodFill(output,Point(0,0),Scalar(255));
    floodFill(output,Point(0,0),Scalar(100));
    output = output != 100;
}

