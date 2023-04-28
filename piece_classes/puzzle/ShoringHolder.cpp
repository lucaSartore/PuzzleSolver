//
// Created by luca on 4/27/23.
//

#include "ShoringHolder.h"
#include "point_indexes.h"
#include <math.h>
using namespace cv;

ShoringHolder::ShoringHolder(): Holder(){}

ShoringHolder::ShoringHolder(PieceImage *piece_, int rotation): Holder(piece_, rotation){
    // copying the image
    rotated_image = get_image().clone();
    // coping the 4 points
    for(int i=0; i<4; i++){
        rotated_points[i] = get_point(i, false);
    }
}

void ShoringHolder::align_to(ShoringHolder &other, Direction direction) {
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

            this_p1 = get_rotated_point(TOP_LEFT_PIECE_CORNER);
            this_p2 = get_rotated_point(TOP_RIGHT_PIECE_CORNER);

            other_p1 = other.get_rotated_point(BOTTOM_LEFT_PIECE_CORNER);
            other_p2 = other.get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER);

            break;
        case RIGHT:

            this_p1 = get_rotated_point(TOP_RIGHT_PIECE_CORNER);
            this_p2 = get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER);

            other_p1 = other.get_rotated_point(TOP_LEFT_PIECE_CORNER);
            other_p2 = other.get_rotated_point(BOTTOM_LEFT_PIECE_CORNER);

            break;
        case DOWN:

            this_p1 = get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER);
            this_p2 = get_rotated_point(BOTTOM_LEFT_PIECE_CORNER);

            other_p1 = other.get_rotated_point(TOP_RIGHT_PIECE_CORNER);
            other_p2 = other.get_rotated_point(TOP_LEFT_PIECE_CORNER);

            break;
        case LEFT:

            this_p1 = get_rotated_point(BOTTOM_LEFT_PIECE_CORNER);
            this_p2 = get_rotated_point(TOP_LEFT_PIECE_CORNER);

            other_p1 = other.get_rotated_point(BOTTOM_RIGHT_PIECE_CORNER);
            other_p2 = other.get_rotated_point(TOP_RIGHT_PIECE_CORNER);

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

cv::Point ShoringHolder::get_rotated_point(int index) {
    assert(index>=0);
    assert(index<4);
    return rotated_points[index];
}

void ShoringHolder::rotate_by(float angle) {
    angle *= 180/CV_PI;

    int res_x = rotated_image.size().width;
    int res_y = rotated_image.size().height;

    Point center = Point (res_x/2, res_y/2);

    // Create a transformation matrix to rotate and move the input image
    // docs: https://docs.opencv.org/3.4/da/d54/group__imgproc__transform.html#gafbbc470ce83812914a70abfb604f4326
    cv::Mat transform = cv::getRotationMatrix2D(center, angle, 1.0);
    //transform.at<double>(0, 2) += ROTATION_RESOLUTION/2 - center.x;
    //transform.at<double>(1, 2) += ROTATION_RESOLUTION/2 - center.y;

    Mat new_rotated_image;

    // Apply the transformation to the input image and put it in the output image
    warpAffine(rotated_image, new_rotated_image, transform, rotated_image.size());

    //put the piece back on place
    rotated_image = new_rotated_image;

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

cv::Point ShoringHolder::get_rotated_point_with_offset(int index) {
    return get_rotated_point(index) + get_offset();
}

void ShoringHolder::move_to(ShoringHolder &other, Direction direction) {

    Point this_side_point;
    Point other_side_point;
    switch (direction) {
        case UP:

            this_side_point = (get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER) +
                            get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER))/2;

            other_side_point = (other.get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER) +
                            other.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER))/2;

            break;
        case RIGHT:

            this_side_point = (get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER) +
                               get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER))/2;

            other_side_point = (other.get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER) +
                                other.get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER))/2;

            break;
        case DOWN:

            this_side_point = (get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER) +
                               get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER))/2;

            other_side_point = (other.get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER) +
                                other.get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER))/2;

            break;
        case LEFT:

            this_side_point = (get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER) +
                               get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER))/2;

            other_side_point = (other.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER) +
                                other.get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER))/2;

            break;
        default:
            throw std::runtime_error("unknown direction!");
    }
    Point offset = other_side_point-this_side_point;
    set_offset(get_offset()+offset);
}

cv::Point ShoringHolder::get_rotated_center() {
    return (
            get_rotated_point(0)+
            get_rotated_point(1)+
            get_rotated_point(2)+
            get_rotated_point(3)
    )/4;
}

cv::Point ShoringHolder::get_rotated_center_with_offset() {
    return get_rotated_center()+get_offset();
}

cv::Mat ShoringHolder::get_rotated_image() {
    return rotated_image.clone();
}

void ShoringHolder::align_to(ShoringHolder &top_piece, ShoringHolder &left_piece) {
    // same concept as the other align_to function, but this time using one corner per piece as diagonal
    Point others_p1 = left_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER);
    Point others_p2 = top_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER);
    Point  this_p1 = get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER);
    Point  this_p2 = get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER);

    // vector to calculate the rotation angle of the side of this piece, and of the other piece
    Point this_vector = this_p2-this_p1;
    Point other_vector =others_p2-others_p1;

    float this_angle = atan2((float)this_vector.y,(float)this_vector.x);
    float other_angle = atan2((float)other_vector.y,(float)other_vector.x);

    float to_rotate = -other_angle + this_angle;



    // rotate the image and the points
    rotate_by(to_rotate);
}

void ShoringHolder::move_to(ShoringHolder &top_piece, ShoringHolder &left_piece) {
    // same concept as the other move_to function, but this time using the center as offset
    Point others_p1 = left_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER);
    Point others_p2 = top_piece.get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER);
    Point this_p1 = get_rotated_point(BOTTOM_LEFT_PIECE_CORNER);
    Point this_p2 = get_rotated_point(TOP_RIGHT_PIECE_CORNER);

    Point others_middle = (others_p1+others_p2)/2;
    Point this_middle = (this_p1+this_p2)/2;

    Point offset = others_middle- this_middle;
    set_offset(offset);
}
