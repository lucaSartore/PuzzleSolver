//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_HOLDER_H
#define PIECECLASS_HOLDER_H

#include "../piece/Piece.h"
#include "../piece/Side.h"

/// the possible directions in the puzzle array
typedef enum Direction{UP, LEFT,DOWN, RIGHT} Direction;

/// the minus sign is used to find the opposite direction to another
/// for examole -UP == DOWN, -LEFT == RIGHT, and so on
Direction operator-(Direction direction);
std::ostream& operator<<(std::ostream & os, Direction direction);


class Holder {
public:
    /// say if this holder is an unknown holder or not
    virtual bool is_unknown();
    /// say if this holder is an outside holder or not
    virtual bool is_outside();
    /// return the compatibility of this piece with the others
    virtual float check_compatibility(Holder* up, Holder* down, Holder* left, Holder* right);
    /// return the side on the specified direction,
    /// it returns null ptr if the holder is an unknown holder
    virtual Side* get_side(Direction direction);
    Holder() = default;
    virtual ~Holder() = default;
    virtual char get_debug_view() const;
};


#endif //PIECECLASS_HOLDER_H