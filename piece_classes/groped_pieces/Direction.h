//
// Created by luca on 4/20/23.
//

#ifndef PIECECLASS_DIRECTION_H
#define PIECECLASS_DIRECTION_H


#include <iostream>

/// the possible directions in the puzzle array
typedef enum Direction{UP, LEFT,DOWN, RIGHT} Direction;

/// the minus sign is used to find the opposite direction to another
/// for examole -UP == DOWN, -LEFT == RIGHT, and so on
Direction operator-(Direction direction);
std::ostream& operator<<(std::ostream & os, Direction direction);



#endif //PIECECLASS_DIRECTION_H
