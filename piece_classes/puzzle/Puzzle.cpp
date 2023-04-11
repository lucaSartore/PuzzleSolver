//
// Created by luca on 4/4/23.
//

#include "Puzzle.h"


#include <utility>

Puzzle::Puzzle(std::string path, int number_of_pieces_) {

    array = PieceArray();

    // alloc space for the pieces
    pieces = new Piece[number_of_pieces_];

    // set the number of pieces
    number_of_pieces = number_of_pieces_;

    // load all the pieces one by one
    Piece::set_origin_path(std::move(path));
    for(int i=0; i<number_of_pieces_;i++){
        pieces[i] = Piece(i);
    }
}

Puzzle::~Puzzle() {
    // free all the pieces from ram;
    delete[] pieces;
}
