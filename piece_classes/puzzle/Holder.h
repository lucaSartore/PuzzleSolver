//
// Created by luca on 4/27/23.
//

#ifndef PIECECLASS_HOLDER_H
#define PIECECLASS_HOLDER_H

#include "PieceImage.h"
#include <opencv2/opencv.hpp>
#include "../groped_pieces/Direction.h"


/*
 * the holder is a struct that contains a piece, and adds information, about his orientation, and his position isnide
 * a piece array. it is the basic structure, that will be extend by PreviewHolder, used fot preview
 * and from ShoringHolder that will be used to day if a piece array is compatible or not
 * */

class Holder {
private:
    cv::Point offset;
    PieceImage* piece;
    int orientation;
    cv::Scalar color;
public:
    /// set the offset of the piece;
    void set_offset(cv::Point p);
    /// get the offset of the piece;
    cv::Point get_offset();
    /// zero parameter constrictor
    Holder();
    /// constructor
    Holder(PieceImage *piece_, int orientation_);
    /// destructor
    ~Holder() = default;
    /// return the resized image held in this holder
    cv::Mat get_image_resized();
    /// return the non resized image held in this holder
    cv::Mat get_image();
    /// return the coordinates (relative to the image obtained with `get_preview_image`) of the offset
    /// of the side at the specified direction
    cv::Point get_side_center(Direction direction,bool resized);
    /// return the coordinates (relative to the image obtained with `get_preview_image`) of the offset of the piece
    cv::Point get_center(bool resized);
    /// same as `get_side_center` but keeps track of where the piece is placed in the piece array using the offset
    cv::Point get_side_center_with_offset(Direction direction, bool resized);
    /// return the coordinates of one of the 4 points of the image, based on the specified index
    cv::Point  get_point(int index,bool resized);
    /// return the pointer to the original piece
    PieceImage* get_piece();
    /// set the color of the current piece;
    void set_color(cv::Scalar new_color);
    /// return the current color of the piece;
    cv::Scalar get_color();
};


#endif //PIECECLASS_HOLDER_H
