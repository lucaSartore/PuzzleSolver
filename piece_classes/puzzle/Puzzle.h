//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PUZZLE_H
#define PIECECLASS_PUZZLE_H

#include "PieceArray.h"
#include "../piece/Piece.h"
#include <list>
#include "Holder.h"

/// the maximum number of piece that will be considered of every spot
/// if more pieces fit they will be excluded
#define MAX_FITS_FOR_POSITION 10

/// the minimum compatibility percentage that a piece needs to have in order to be considered a fit
#define DEFAULT_MIN_COMPATIBILITY 0.5


class Puzzle {
public:
    // the array containing the current state of the puzzle
    PieceArray array;
    // all the pieces of te puzzle
    Piece *pieces;
    // the total number of pieces of the current puzzle
    int number_of_pieces;
    // a value that define the minimum percentage of compatibility a piece needs to have in order to be considered a good candidate
    float min_compatibility;
    // a vector containing all the available pieces (aka: the one that are not already positioned in the puzzle)
    std::vector<Piece*> available_pieces;
public:
    /// constructor, parameters are;
    /// where to find the information about the pieces
    /// - path: where to find the information about the pieces
    /// - number_of_pieces: the number of pieces of the puzzle
    Puzzle(std::string path, int number_of_pieces_);
    Puzzle() = delete;
    ~Puzzle();
    /// set the minimum compatibility a piece need to have in order
    /// to be considered a possible candidate for a spot
    void set_min_compatibility(float new_value);
    /// return a list containing the best fit for the asked position
    /// the list is sorted form best fit to worst fit
    std::list<std::tuple<float,std::shared_ptr<Holder>>> get_best_fits(int x, int y);
};


#endif //PIECECLASS_PUZZLE_H
