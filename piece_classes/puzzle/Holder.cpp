//
// Created by luca on 4/4/23.
//

#include "Holder.h"

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

void Holder::set_center(cv::Point p) {
    center = p;
}

cv::Point Holder::get_center() {
    return center;
}

Holder::Holder() {
    center = cv::Point(0,0);
}

Direction operator-(Direction direction){
    switch (direction) {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case RIGHT:
            return LEFT;
        case LEFT:
            return RIGHT;
        default:
            throw std::invalid_argument("unknown direction");
    }
}
std::ostream& operator<<(std::ostream & os, Direction direction){
    switch (direction) {
        case UP:
            return os << "UP";
        case DOWN:
            return os << "DOWN";
        case RIGHT:
            return os << "RIGHT";
        case LEFT:
            return os << "LEFT";
        default:
            throw std::invalid_argument("unknown direction");
    }
}