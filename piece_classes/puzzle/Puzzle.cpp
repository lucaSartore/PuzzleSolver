//
// Created by luca on 4/4/23.
//

#include "Puzzle.h"


#include <utility>
#include "PieceHolder.h"
#include <memory>
#include <algorithm>

using namespace std;

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

    // inserting the pieces in the available_pieces
    for(int i=0; i<number_of_pieces; i++){
        available_pieces.push_back(pieces+i);
    }
}

Puzzle::~Puzzle() {
    // free all the pieces from ram;
    delete[] pieces;
}

void Puzzle::set_min_compatibility(float new_value) {
    min_compatibility = new_value;
}

std::list<std::tuple<float,shared_ptr<Holder>>> Puzzle::get_best_fits(int x, int y) {
    std::list<std::tuple<float,shared_ptr<Holder>>> best_fits;

    // Iterate through all the available pieces
    for (auto piece : available_pieces) {
        for(int orientation = 0; orientation<4; orientation++){

            shared_ptr<Holder>  to_test = shared_ptr<Holder>(new PieceHolder(piece,orientation));

            float compatibility = array.check_compatibility(x,y,to_test.get());

            // Only consider pieces with a compatibility score greater than min_compatibility
            if (compatibility >= min_compatibility) {
                // create the tuple
                auto to_insert = tuple<float, shared_ptr<Holder>>(compatibility,to_test);
                // Add the compatibility score and the piece to the list of best fits
                best_fits.emplace_back(to_insert);
            }
        }
    }

    // Sort the list of best fits by compatibility score in descending order

    best_fits.sort([]( std::tuple<float, std::shared_ptr<Holder>>& a,  std::tuple<float, std::shared_ptr<Holder>>& b) {
        return std::get<0>(a) > std::get<0>(b);
    });

    // Truncate the list to contain only the top 10 best fits
    if (best_fits.size() > 10) {
        best_fits.resize(10);
    }

    return best_fits;
}

bool Puzzle::solve_puzzle_recursive(Puzzle &puzzle) {
    return false;
}
