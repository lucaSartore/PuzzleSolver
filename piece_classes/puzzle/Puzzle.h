//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PUZZLE_H
#define PIECECLASS_PUZZLE_H

#include "PieceArray.h"
#include "../graphic_piece/PieceShape.h"
#include <list>
#include "Holder.h"

// short name
typedef PieceShape Piece;

/// the maximum number of piece that will be considered of every spot
/// if more pieces fit they will be excluded
#define MAX_FITS_FOR_POSITION 10

/// the minimum compatibility percentage that a piece needs to have in order to be considered a fit
#define DEFAULT_MIN_COMPATIBILITY 0.5

class PuzzlePointer{
private:
    std::list<std::tuple<int,int>> list_x_y;
    // the coordinates the pointer is pointing to right now
    int x,y;
    PieceArray* reference_array;
public:
    PuzzlePointer();
    /// build a puzzle pointer structure, need to pass a pointer to the array this pointer is referring to
    explicit PuzzlePointer(PieceArray* reference_array_);
    /// return the current x position of the pointer
    [[nodiscard]] int get_x() const;
    /// return the current y position of the pointer
    [[nodiscard]] int get_y() const;

    /// advance the position by one
    /// the function return false if the puzzle is finished and is impossible to advance further
    /// it returns true otherwise
    /// it also throws an error if it find out that he is pointing to a point that has no
    /// clear next element, or a point with no actual piece in it

    bool next();
    /// retreat the position by one
    /// the function return false if you try to call it when he is pointing to 0,0 (aka the beginning)
    /// it returns true otherwise
    bool prev();
};

std::ostream & operator<<(std::ostream& os, const PuzzlePointer & pp);

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
    static bool solve_puzzle_recursive(Puzzle& puzzle);
    // show the puzzle to the user
    void show();
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
    /// return a list_x_y containing the best fit for the asked position
    /// the list_x_y is sorted form best fit to worst fit
    std::list<std::tuple<float,std::shared_ptr<Holder>>> get_best_fits(int x, int y);
    /// slove the puzzle
    bool solve();
};



#endif //PIECECLASS_PUZZLE_H
