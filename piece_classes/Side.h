//
// Created by luca on 3/18/23.
//

#ifndef PUZZLE_SOLVER_CORNER_H
#define PUZZLE_SOLVER_CORNER_H


#include <opencv2/opencv.hpp>

using namespace cv;

class Side {
private:
    // a image containing the shape of the border ready compared it with others
    Mat border_shape;
    // is a mask containing the part of the border that needs to be checked for compatibility
    Mat border_mask;
public:
    Side();
    // constructor
    Side(Mat& piece, Point p1, Point p2);

    // a comparing function that returns a percentage thad indicate how match the 2 pieces are comparable
    float compare_to(Side &other);
};


#endif //PUZZLE_SOLVER_CORNER_H
