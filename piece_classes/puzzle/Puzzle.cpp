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

    min_compatibility = DEFAULT_MIN_COMPATIBILITY;
}

Puzzle::~Puzzle() {
    // free all the pieces from ram;
    delete[] pieces;
}

void Puzzle::set_min_compatibility(float new_value) {
    min_compatibility = new_value;
}

std::list<std::tuple<float, Holder>> Puzzle::get_best_fits(int x, int y) {
    return std::list<std::tuple<float, Holder>>();
}
