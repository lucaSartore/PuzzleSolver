//
// Created by luca on 4/4/23.
//

#include "Holder.h"

//#include <utility>



void Holder::set_offset(cv::Point p) {
    offset = p;
}

cv::Point Holder::get_offset() {
    return offset;
}

Holder::Holder(Piece *piece_, int orientation_) {
    assert(piece_ != nullptr);
    assert(orientation_>=0 && orientation_<4);

    piece = piece_;
    orientation = orientation_;
    offset = cv::Point(0, 0);
    color = cv::Scalar(255,255,255);
}

void Holder::set_color(cv::Scalar new_color) {
    color = std::move(new_color);
}

cv::Scalar Holder::get_color() {
    return color;
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


cv::Point Holder::get_side_center(Direction direction) {
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
    cv::Point p1 = get_point((offset)%4);
    cv::Point p2 = get_point((offset+1)%4);
    return (p1+p2)/2;
}

cv::Point Holder::get_point(int index) {
    assert(index>=0);
    assert(index<4);
    cv::Point og_point = piece->get_point((orientation+index)%4);
    int res_x = piece->get_preview_x_res();
    int res_y = piece->get_preview_y_res();
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

cv::Point Holder::get_side_center_with_offset(Direction direction) {
    return this->get_side_center(direction) + this->get_offset();
}

cv::Point Holder::get_center() {
    cv::Point p1 = get_point(0);
    cv::Point p2 = get_point(1);
    cv::Point p3 = get_point(2);
    cv::Point p4 = get_point(3);
    return (p1+p2+p3+p4)/4;
}

Piece *Holder::get_piece() {
    return piece;
}

Holder::Holder() {
    offset = Point();
    color = Scalar(255,255,255);
    piece = nullptr;
    orientation = 0;
}

