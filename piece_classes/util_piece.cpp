//
// Created by luca on 3/19/23.
//

#include "util_piece.h"
#include <cmath>

float find_angle_of_2_points(Point p1, Point p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return atan2(dy, dx);
}