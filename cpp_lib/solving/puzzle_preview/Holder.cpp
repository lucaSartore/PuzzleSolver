//
// Created by luca on 4/27/23.
//

#include "Holder.h"
#include "point_indexes.h"
#include "../constants.h"
using namespace cv;

bool Holder::default_resize = false;

void Holder::set_offset(cv::Point p, bool resize) {
    if(resize){
        offset = p*PREVIEW_RESIZE_FACTOR;
    }else{
        offset = p;
    }
}

cv::Point Holder::get_offset(bool resize) {
    if(resize){
        return offset/PREVIEW_RESIZE_FACTOR;
    }else{
        return offset;
    }
}

Holder::Holder(PieceImage *piece_, int orientation_) {
    assert(piece_ != nullptr);
    assert(orientation_>=0 && orientation_<4);

    color = Scalar (255,255,255);
    piece = piece_;
    orientation = orientation_;
    offset = cv::Point(0, 0);

    // take the original imag and rotate it
    int rotate_code = -1;
    switch (orientation) {
        case 0:
            // no need for rotation;
            rotated_image = piece->get_image().clone();
            break;
        case 1:
            rotate_code = cv::ROTATE_90_COUNTERCLOCKWISE;
            break;
        case 2:
            rotate_code = cv::ROTATE_180;
            break;
        case 3:
            rotate_code = cv::ROTATE_90_CLOCKWISE;
            break;
        default:
            throw std::runtime_error("unknown orientation");
    }
    if(rotate_code != -1){
        cv::rotate(piece->get_image(),rotated_image,rotate_code);
    }
    // also save the rotated image
    cv::resize(rotated_image,rotated_image_resize,rotated_image.size()/PREVIEW_RESIZE_FACTOR);

    // save the points
    int res_x = piece->get_full_x_res();
    int res_y = piece->get_full_x_res();
    for(int point_index = 0; point_index<4; point_index++){
        Point p = piece->get_point((orientation+point_index)%4);
        int x,y;
        switch (orientation) {
            case 0:
                // no rotation
                x = p.x;
                y = p.y;
                break;
            case 1:
                // 90 deg counter clock wise
                x = p.y;
                y = res_y-p.x;
                break;
            case 2:
                // 180 deg
                x = res_x-p.x;
                y = res_y-p.y;
                break;
            case 3:
                // 90 deg clock wise
                x = res_x-p.y;
                y = p.x;
                break;
            default:
                throw std::runtime_error("unknown orientation");
        }
        rotated_points[point_index] = Point(x,y);
    }

}


cv::Point Holder::get_side_center(Direction direction, bool resized) {
    int offset;
    switch (direction) {
        case UP:
            offset=0;
            break;
        case RIGHT:
            offset=1;
            break;
        case DOWN:
            offset=2;
            break;
        case LEFT:
            offset=3;
            break;
        default:
            throw std::runtime_error("unknown direction");
    }
    cv::Point p1 = get_rotated_point((offset)%4,resized);
    cv::Point p2 = get_rotated_point((offset+1)%4,resized);
    return (p1+p2)/2;
}


cv::Point Holder::get_side_center_with_offset(Direction direction, bool resized) {
    return this->get_side_center(direction,resized) + this->get_offset(resized);
}


PieceImage *Holder::get_piece() {
    return piece;
}

Holder::Holder() {
    offset = Point();
    piece = nullptr;
    orientation = 0;
}

cv::Mat Holder::get_image(bool resized) {
    if(resized){
        return rotated_image_resize.clone();
    }else{
        return rotated_image.clone();
    }
}

void Holder::set_color(cv::Scalar new_color) {
    color = std::move(new_color);
}

cv::Scalar Holder::get_color() {
    return color;
}

int Holder::get_orientation() {
    return orientation;
}

int Holder::get_id() {
    return piece->get_id();
}

void Holder::set_resize(bool new_default_resize) {
    default_resize = new_default_resize;
}

cv::Point Holder::get_rotated_point(int index, bool resize) {
    assert(index>=0);
    assert(index<4);
    if(resize){
        return rotated_points[index]/PREVIEW_RESIZE_FACTOR;
    }else{
        return rotated_points[index];
    }
}

cv::Point Holder::get_rotated_point_with_offset(int index, bool resize) {
    return get_rotated_point(index,resize) + get_offset(resize);
}

cv::Point Holder::get_rotated_center_with_offset(bool resize) {
    return get_rotated_center(resize)+get_offset(resize);
}

cv::Point Holder::get_rotated_center(bool resize) {
    return (
                   get_rotated_point(0,resize)+
                   get_rotated_point(1,resize)+
                   get_rotated_point(2,resize)+
                   get_rotated_point(3,resize)
           )/4;
}

void Holder::rotate_by(float angle) {
    angle *= 180/CV_PI;

    int res_x = rotated_image.size().width;
    int res_y = rotated_image.size().height;

    Point center = Point (res_x/2, res_y/2);

    // Create a transformation matrix to rotate and move the input image
    // docs: https://docs.opencv.org/3.4/da/d54/group__imgproc__transform.html#gafbbc470ce83812914a70abfb604f4326
    cv::Mat transform = cv::getRotationMatrix2D(center, angle, 1.0);

    Mat new_rotated_image;

    // Apply the transformation to the input image and put it in the output image
    warpAffine(rotated_image, new_rotated_image, transform, rotated_image.size());

    //put the piece back on place
    rotated_image = new_rotated_image;
    resize(rotated_image,rotated_image_resize,rotated_image.size()/PREVIEW_RESIZE_FACTOR);

    // transform the coordinates as well

    // translating my coordinates to the new form
    std::vector<Point2f> points_vector = std::vector<Point2f>(),  points_vector_transformed = std::vector<Point2f>();
    for(auto p: rotated_points){
        points_vector.push_back((Point2f)p);
    }
    cv::transform(points_vector,points_vector_transformed,transform);
    for(int i=0; i<4; i++){
        rotated_points[i] = points_vector_transformed[i];
    }
}


void Holder::move_to(Holder &other, Direction direction, bool resized) {

    Point this_side_point;
    Point other_side_point;
    switch (direction) {
        case UP:

            this_side_point = (get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER,resized) +
                               get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER,resized))/2;

            other_side_point = (other.get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER,resized) +
                                other.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER,resized))/2;

            break;
        case RIGHT:

            this_side_point = (get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER,resized) +
                               get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER,resized))/2;

            other_side_point = (other.get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER,resized) +
                                other.get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER,resized))/2;

            break;
        case DOWN:

            this_side_point = (get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER,resized) +
                               get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER,resized))/2;

            other_side_point = (other.get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER,resized) +
                                other.get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER,resized))/2;

            break;
        case LEFT:

            this_side_point = (get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER,resized) +
                               get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER,resized))/2;

            other_side_point = (other.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER,resized) +
                                other.get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER,resized))/2;

            break;
        default:
            throw std::runtime_error("unknown direction!");
    }
    Point offset = other_side_point-this_side_point;
    set_offset(get_offset(resized)+offset,resized);
}

void Holder::move_to(Holder &top_piece, Holder &left_piece,bool resized) {
    // same concept as the other move_to function, but this time using the center as offset
    Point others_p1 = left_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER,resized);
    Point others_p2 = top_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER,resized);
    Point this_p1 = get_rotated_point(BOTTOM_LEFT_PIECE_CORNER,resized);
    Point this_p2 = get_rotated_point(TOP_RIGHT_PIECE_CORNER,resized);

    Point others_middle = (others_p1+others_p2)/2;
    Point this_middle = (this_p1+this_p2)/2;

    Point offset = others_middle- this_middle;
    set_offset(offset,resized);
}

void Holder::align_to(Holder &other, Direction direction) {
    /*
     * this function needs to rotate the current piece with one other, to do that it needs to rotate it
     * bt an angle, this function calculate the angle, by making the difference between the angle formed
     * by the 2 points of the side to match of this piece with the angle of the 2 points of the side to match
     * of the other piece
     * */

    // this p1 is the first corner (clockwise order) of this piece on the analyzed side
    // this p2 is the second corner (clockwise order) of this piece on the analyzed side
    // other p1 is the first corner (counterclockwise order) of the other piece on the analyzed side (witch is the opposite of the side of `this`)
    // other p2 is the second corner (counterclockwise order) of the other piece on the analyzed side (witch is the opposite of the side of `this`)
    Point this_p1,this_p2,other_p1,other_p2;
    switch (direction) {
        case UP:

            this_p1 = get_rotated_point(TOP_LEFT_PIECE_CORNER, false);
            this_p2 = get_rotated_point(TOP_RIGHT_PIECE_CORNER, false);

            other_p1 = other.get_rotated_point(BOTTOM_LEFT_PIECE_CORNER, false);
            other_p2 = other.get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER, false);

            break;
        case RIGHT:

            this_p1 = get_rotated_point(TOP_RIGHT_PIECE_CORNER, false);
            this_p2 = get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER, false);

            other_p1 = other.get_rotated_point(TOP_LEFT_PIECE_CORNER, false);
            other_p2 = other.get_rotated_point(BOTTOM_LEFT_PIECE_CORNER, false);

            break;
        case DOWN:

            this_p1 = get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER, false);
            this_p2 = get_rotated_point(BOTTOM_LEFT_PIECE_CORNER, false);

            other_p1 = other.get_rotated_point(TOP_RIGHT_PIECE_CORNER, false);
            other_p2 = other.get_rotated_point(TOP_LEFT_PIECE_CORNER, false);

            break;
        case LEFT:

            this_p1 = get_rotated_point(BOTTOM_LEFT_PIECE_CORNER, false);
            this_p2 = get_rotated_point(TOP_LEFT_PIECE_CORNER, false);

            other_p1 = other.get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER, false);
            other_p2 = other.get_rotated_point(TOP_RIGHT_PIECE_CORNER, false);

            break;
        default:
            throw std::runtime_error("unknown direction!");
    }

    // vector to calculate the rotation angle of the side of this piece, and of the other piece
    Point this_vector = this_p2-this_p1;
    Point other_vector =other_p2-other_p1;

    float this_angle = atan2((float)this_vector.y,(float)this_vector.x);
    float other_angle = atan2((float)other_vector.y,(float)other_vector.x);

    float to_rotate = -other_angle + this_angle;

    // rotate the image and the points
    rotate_by(to_rotate);
}

void Holder::align_to(Holder &top_piece, Holder &left_piece) {
    // same concept as the other align_to function, but this time using one corner per piece as diagonal
    Point others_p1 = left_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER, false);
    Point others_p2 = top_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER, false);
    Point  this_p1 = get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER, false);
    Point  this_p2 = get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER, false);

    // vector to calculate the rotation angle of the side of this piece, and of the other piece
    Point this_vector = this_p2-this_p1;
    Point other_vector =others_p2-others_p1;

    float this_angle = atan2((float)this_vector.y,(float)this_vector.x);
    float other_angle = atan2((float)other_vector.y,(float)other_vector.x);

    float to_rotate = -other_angle + this_angle;



    // rotate the image and the points
    rotate_by(to_rotate);
}

