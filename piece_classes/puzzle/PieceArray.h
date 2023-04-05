//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PIECEARRAY_H
#define PIECECLASS_PIECEARRAY_H

#include "Holder.h"
#include "vector"
#include <iostream>



/*

 the puzzle must grow automatically when inserting a piece that is bigger than is outside the maximum dimension by 1
 the puzzle must throw an error if i try to place a piece that is outside of the dimension of the puzzle by 2 or more
 the puzzle must shrink automatically when calling a remove piece at the corner
 the puzzle must trow an error if calling a remove piece that is not in a corner



 */


class PieceArray {
private:
    int dim_x;
    int dim_y;
    std::vector<std::vector<Holder*>> pieces;
    void check_indexes(int x, int y) const;
public:
    /// returns the x dimension of the 2d array
    int get_dim_x() const;
    /// returns the y dimension of the 2d array
    int get_dim_y() const;
    /// create an empty piece array
    PieceArray();
    /// return a piece_holder in one of the many position
    const Holder& get(int x, int y) const;
    /// set a piece_holder in one position
    void set(int x, int y, Holder &to_be_set);
    /// remove a piece form the puzzle
    void remove(int x, int y);
    /// grow the array by 1 in the X dimension
    void grow_x();
    /// grow the array by 1 in the Y dimension
    void grow_y();
    /// un grow the array by 1 in the X dimension
    void un_grow_x();
    /// un grow the array by 1 in the Y dimension
    void un_grow_y();

};


std::ostream& operator<<(std::ostream& os, const PieceArray& pa);



#endif //PIECECLASS_PIECEARRAY_H
