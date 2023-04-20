//
// Created by luca on 4/20/23.
//

#include "Direction.h"

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