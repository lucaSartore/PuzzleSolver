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

class PuzzlePointer{
private:
    // the coordinates the pointer is pointing to right now
    int x,y;
    PieceArray* reference_array;
public:
    /// build a puzzle pointer structure, need to pass a pointer to the array this pointer is referring to
    PuzzlePointer(PieceArray* reference_array_);
    /// return the current x position of the pointer
    int get_x();
    /// return the current y position of the pointer
    int get_y();
    /// advance the position by one
    /// it throws an error if the puzzle is finished and is impossible to advance further
    /// it also throws an error if it find out that he is pointing to a point that has no
    /// clear next element, or a point with no actual piece in it
    void next();
    /// retreat the position by one
    /// it throws an error if you try to call it when he is pointing to 0,0 (aka the beginning)
    /// it also throws an error if it find out that he is pointing to a point that has no
    /// clear next element, or a point with no actual piece in it
    void prev();
};

class Puzzle {
public:
    PuzzlePointer puzzle_pointer;
    // the array containing the current state of the puzzle
    PieceArray array;
    // all the pieces of te puzzle
    Piece *pieces;
    // the total number of pieces of the current puzzle
    int number_of_pieces;
    // a value that define the minimum percentage of compatibility a piece needs to have in order to be considered a good candidate
    float min_compatibility;
    // a vector containing all the available pieces (aka: the one that are not already positioned in the puzzle)
    std::list<Piece*> available_pieces;
    // inductive function for solving a puzzle, it returns true if it manage to solve it false if not
    static bool solve_puzzle_recursive(Puzzle& puzzle, int x_to_);
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
