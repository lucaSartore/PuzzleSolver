//
// Created by luca on 4/4/23.
//

#include "Holder.h"

#include <utility>

char Holder::get_debug_view() const{
    return 'H';
}

bool Holder::is_unknown() {
    return false;
}

float Holder::check_compatibility(Holder *up, Holder *down, Holder *left, Holder *right) {
    return 0;
}

Side *Holder::get_side(Direction direction) {
    return nullptr;
}

bool Holder::is_outside() {
    return false;
}

bool Holder::is_a_piece() {
    return false;
}

void Holder::set_offset(cv::Point p) {
    offset = p;
}

cv::Point Holder::get_offset() {
    return offset;
}

Holder::Holder() {
    offset = cv::Point(0, 0);
    color = cv::Scalar(255,255,255);
}

void Holder::set_color(cv::Scalar new_color) {
    color = std::move(new_color);
}

cv::Scalar Holder::get_color() {
    return color;
}
