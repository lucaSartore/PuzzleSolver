//
// Created by luca on 4/27/23.
//

#ifndef PIECECLASS_SHORINGHOLDER_H
#define PIECECLASS_SHORINGHOLDER_H


#include "PieceImage.h"
#include "Holder.h"
#include <opencv2/opencv.hpp>


class ShoringHolder: public Holder{
private:
    /// the image after rotation
    cv::Mat rotated_image;
    /// the coordinates of the 4 corners after rotations
    cv::Point rotated_points[4];
public:
    /// default constructor
    ShoringHolder();
    /// one parameter constructor
    ShoringHolder(PieceImage* piece_, int rotation);
    /// take this piece and rotate him to be aligned with the other piece with the specified direction
    void align_to(ShoringHolder& other, Direction direction);
    /// take this piece and rotate him to be aligned with the the 2 pieces at the top and at the left
    void align_to(ShoringHolder& top_piece, ShoringHolder& left_piece);
    /// move the current so that it will have the other piece at the specified direction
    void move_to(ShoringHolder& other, Direction direction);
    /// move the current so that it will two pieces to the top and left
    void move_to(ShoringHolder& top_piece, ShoringHolder& left_piece);
};


#endif //PIECECLASS_SHORINGHOLDER_H
