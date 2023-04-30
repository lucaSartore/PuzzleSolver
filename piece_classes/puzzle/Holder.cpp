//
// Created by luca on 4/27/23.
//

#include "Holder.h"
#include "point_indexes.h"

using namespace cv;

void Holder::set_offset(cv::Point p) {
    offset = p;
}

cv::Point Holder::get_offset() {
    return offset;
}

Holder::Holder(PieceImage *piece_, int orientation_) {
    assert(piece_ != nullptr);
    assert(orientation_>=0 && orientation_<4);

    piece = piece_;
    orientation = orientation_;
    offset = cv::Point(0, 0);
}

cv::Mat Holder::get_image_resized() {
    int rotate_code;
    switch (orientation) {
        case 0:
            // no need for rotation;
            return piece->get_image_resized().clone();
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
    cv::Mat to_return;
    cv::rotate(piece->get_image_resized(),to_return,rotate_code);
    return to_return;
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
    cv::Point p1 = get_point((offset)%4,resized);
    cv::Point p2 = get_point((offset+1)%4,resized);
    return (p1+p2)/2;
}

cv::Point Holder::get_point(int index, bool resized) {
    assert(index>=0);
    assert(index<4);

    cv::Point og_point;
    int res_x;
    int res_y;
    if(resized){
        og_point = piece->get_point_resized((orientation+index)%4);
        res_x = piece->get_preview_x_res();
        res_y = piece->get_preview_y_res();
    }else{
        og_point = piece->get_point((orientation+index)%4);
        res_x = piece->get_full_x_res();
        res_y = piece->get_full_y_res();
    }

    int x,y;
    switch (orientation) {
        case 0:
            // no rotation
            x = og_point.x;
            y = og_point.y;
            break;
        case 1:
            // 90 deg counter clock wise
            x = og_point.y;
            y = res_y-og_point.x;
            break;
        case 2:
            // 180 deg
            x = res_x-og_point.x;
            y = res_y-og_point.y;
            break;
        case 3:
            // 90 deg clock wise
            x = res_x-og_point.y;
            y = og_point.x;
            break;
        default:
            throw std::runtime_error("unknown orientation");
    }
    return cv::Point(x,y);

}

cv::Point Holder::get_side_center_with_offset(Direction direction, bool resized) {
    return this->get_side_center(direction,resized) + this->get_offset();
}

cv::Point Holder::get_center(bool resized) {
    cv::Point p1 = get_point(0,resized);
    cv::Point p2 = get_point(1,resized);
    cv::Point p3 = get_point(2,resized);
    cv::Point p4 = get_point(3,resized);
    return (p1+p2+p3+p4)/4;
}

PieceImage *Holder::get_piece() {
    return piece;
}

Holder::Holder() {
    offset = Point();
    piece = nullptr;
    orientation = 0;
}

cv::Mat Holder::get_image() {
    int rotate_code;
    switch (orientation) {
        case 0:
            // no need for rotation;
            return piece->get_image().clone();
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
    cv::Mat to_return;
    cv::rotate(piece->get_image(),to_return,rotate_code);
    return to_return;
}

void Holder::set_color(cv::Scalar new_color) {
    color = std::move(new_color);
}

cv::Scalar Holder::get_color() {
    return color;
}
