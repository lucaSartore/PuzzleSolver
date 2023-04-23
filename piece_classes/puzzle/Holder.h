//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_HOLDER_H
#define PIECECLASS_HOLDER_H

#include "../graphic_piece/PieceShape.h"
#include "../graphic_piece/Side.h"

// shorter alias
typedef PieceShape Piece;

#include <opencv2/opencv.hpp>


#include "../groped_pieces/Direction.h"

class Holder {
private:
    cv::Point offset;
    cv::Scalar color;
    Piece* piece;
    int orientation;
public:
    /// set the offset of the piece;
    void set_offset(cv::Point p);
    /// get the offset of the piece;
    cv::Point get_offset();
    /// set the color of the current piece;
    void set_color(cv::Scalar new_color);
    /// return the current color of the piece;
    cv::Scalar get_color();
    /// zero parameter constrictor
    Holder();
    /// constructor
    Holder(Piece *piece_, int orientation_);
    /// destructor
    ~Holder() = default;
    /// return the image held in this holder
    cv::Mat get_image();
    /// return the coordinates (relative to the image obtained with `get_image`) of the offset
    /// of the side at the specified direction
    cv::Point get_side_center(Direction direction);
    /// return the coordinates (relative to the image obtained with `get_image`) of the offset of the piece
    cv::Point get_center();
    /// same as `get_side_center` but keeps track of where the piece is placed in the piece array using the offset
    cv::Point get_side_center_with_offset(Direction direction);
    /// return the coordinates of one of the 4 points of the image, based on the specified index
    cv::Point  get_point(int index);
    /// return the pointer to the original piece
    Piece* get_piece();
};


#endif //PIECECLASS_HOLDER_H