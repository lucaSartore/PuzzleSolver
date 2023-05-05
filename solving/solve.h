//
// Created by luca on 5/5/23.
//

#ifndef PIECECLASS_SOLVE_H
#define PIECECLASS_SOLVE_H

#include <iostream>

/// this function try many possible combinations of a puzzle, and find the most likely solution(s)
/// the parameters are:
/// - input_path_connections: path where to find the pre-calculated connections of all the pieces
/// - input_path_images: path where to fins the images of the pieces, with the coordinates of the corners
/// - output_path: path where to save the results once found
/// - dim_x: the x dimension of the puzzle to slve
/// - dim_y: the y dimension of the puzzle to solve
/// - number_of_pieces: the total number of pieces (can be higher then dim_x*dim_y, but not lower)
/// - number_of_cores: the number of CPU cores to use for solving, if is 0: auto detect
void solve_puzzle_function(
        std::string input_path_connections,
        std::string input_path_images,
        std::string output_path,
        unsigned int dim_x,
        unsigned int dim_y,
        unsigned int number_of_pieces,
        unsigned int number_of_cores = 0
                );


#endif //PIECECLASS_SOLVE_H
