//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PIECEARRAY_H
#define PIECECLASS_PIECEARRAY_H

#include "PieceHolder.h"
#include "vector"
class PieceArray {
private:
    int dim_x;
    int dim_y;
    std::vector<std::vector<PieceHolder>> pieces;
public:
    /// create an empty piece array
    PieceArray();
    /// return a piece_holder in one of the many position
    PieceHolder& get(int x, int y);
    /// set a piece_holder in one position
    void set(int x, int y, PieceHolder to_be_set);
};


#endif //PIECECLASS_PIECEARRAY_H
