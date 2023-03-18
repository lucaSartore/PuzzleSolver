//
// Created by luca on 3/16/23.
//

#ifndef PROCESSING_UTIL_H
#define PROCESSING_UTIL_H

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

/// show an image and resize ti... ti just save a few lines of code
void show(Mat &m);
/// find the convex hull of a mask, and fill it with white.
void quick_convex_hull(Mat &input, Mat &output);


#endif //PROCESSING_UTIL_H
