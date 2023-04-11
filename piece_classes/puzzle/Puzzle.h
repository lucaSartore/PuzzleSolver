//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PUZZLE_H
#define PIECECLASS_PUZZLE_H

#include "PieceArray.h"
#include "../piece/Piece.h"


class Puzzle {
private:
    PieceArray array;
    Piece *pieces;
    int number_of_pieces;
public:
    /// constructor, parameters are;
    /// where to find the information about the pieces
    /// - path: where to find the information about the pieces
    /// - number_of_pieces: the number of pieces of the puzzle
    Puzzle(std::string path, int number_of_pieces_);
    Puzzle() = delete;
    ~Puzzle();
};


#endif //PIECECLASS_PUZZLE_H
