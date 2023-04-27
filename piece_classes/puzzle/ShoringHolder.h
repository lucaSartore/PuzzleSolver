//
// Created by luca on 4/27/23.
//

#ifndef PIECECLASS_SHORINGHOLDER_H
#define PIECECLASS_SHORINGHOLDER_H


#include "PieceImage.h"
#include <opencv2/opencv.hpp>


class ShoringHolder {
private:
    /// the piece this holder is holding
    PieceImage* piece;
    /// the image after rotation
    cv::Mat rotated_image;
    /// the coordinates of the 4 corners after rotations
    cv::Point rotated_points[4];
    /// the the rotation of the original piece
    int orientation;
    /// how much the center of this piece has been offset in the final image
    cv::Point offset;

public:
    /// default constructor
    ShoringHolder();
    /// one parameter constructor
    ShoringHolder(PieceImage* piece_);
};


#endif //PIECECLASS_SHORINGHOLDER_H
