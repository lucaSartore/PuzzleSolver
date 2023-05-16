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
    /// the image after rotation
    cv::Mat rotated_image;
    /// the image after rotation, and resize
    cv::Mat rotated_image_resize;
    /// the coordinates of the 4 corners after rotations
    cv::Point rotated_points[4];
    /// rotate the rotated image by an angle (in rad) and also change rhe coordinates
    void rotate_by(float angle);
    /// how mutch this piece is moved n relation to the (0,0) of the original image
    cv::Point offset;
    /// pointer to the original piece image
    PieceImage* piece;
    /// orientation of the current piece
    int orientation;
    /// color
    cv::Scalar color;
    /// default behavieur of function (see set_resize);
    static bool default_resize;
public:
    /// set the default behaviour of function (whether to return the resized or the non resized variant of a feature)
    /// if default resize is true all function will return a resized version of the asked feature. and return a non resized
    /// version otherwise
    static void set_resize(bool new_default_resize);
    /// set the offset of the piece;
    void set_offset(cv::Point p, bool resize);
    void set_offset(cv::Point p){set_offset(p, default_resize);};
    /// get the offset of the piece;
    cv::Point get_offset(bool resize);
    cv::Point get_offset(){return get_offset(default_resize);};
    /// zero parameter constrictor
    Holder();
    /// constructor
    Holder(PieceImage *piece_, int orientation_);
    /// destructor
    ~Holder() = default;
    /// return the non resized image held in this holder
    cv::Mat get_image(bool resized);
    cv::Mat get_image(){return get_image(default_resize);};
    /// return the coordinates (relative to the image obtained with `get_image`) of the offset
    /// of the side at the specified direction
    cv::Point get_side_center(Direction direction,bool resized);
    cv::Point get_side_center(Direction direction){return get_side_center(direction,default_resize);};
    /// same as `get_side_center` but keeps track of where the piece is placed in the piece array using the offset
    cv::Point get_side_center_with_offset(Direction direction, bool resized);
    cv::Point get_side_center_with_offset(Direction direction){return get_side_center_with_offset(direction, default_resize);};
    /// take this piece and rotate him to be aligned with the other piece with the specified direction
    void align_to(Holder& other, Direction direction);
    /// take this piece and rotate him to be aligned with the the 2 pieces at the top and at the left
    void align_to(Holder& top_piece, Holder& left_piece);
    /// move the current so that it will have the other piece at the specified direction
    void move_to(Holder& other, Direction direction);
    /// move the current so that it will two pieces to the top and left
    void move_to(Holder& top_piece, Holder& left_piece);
    /// return the point (that has been previously rotated) at the specified index
    cv::Point get_rotated_point(int index, bool resize);
    cv::Point get_rotated_point(int index){return get_rotated_point(index, default_resize);};
    /// return the point (that has been previously rotated) at the specified index
    /// and also contains the offset that has been set
    cv::Point get_rotated_point_with_offset(int index, bool resize);
    cv::Point get_rotated_point_with_offset(int index){return get_rotated_point_with_offset(index, default_resize);};
    /// return the center of the piece, considering the rotation
    cv::Point get_rotated_center(bool resize);
    cv::Point get_rotated_center(){return  get_rotated_center(default_resize);};
    /// return the center of the piece, considering the rotation and the offset
    cv::Point get_rotated_center_with_offset(bool resize);
    cv::Point get_rotated_center_with_offset(){return get_rotated_center_with_offset(default_resize);};
    /// return the point (that has been previously rotated) at the specified index
    /// return the pointer to the original piece
    PieceImage* get_piece();
    /// set the color of the current piece;
    void set_color(cv::Scalar new_color);
    /// return the current color of the piece;
    cv::Scalar get_color();
    /// return the orientation of the piece
    int get_orientation();
    /// return the id of the piece
    int get_id();
};


#endif //PIECECLASS_HOLDER_H
