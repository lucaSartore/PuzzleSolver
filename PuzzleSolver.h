//
// Created by luca on 5/3/23.
//

#ifndef PROCESSING_PUZZLESOLVER_H
#define PROCESSING_PUZZLESOLVER_H

#include <iostream>

/// the current state of the state machine
typedef enum State{
    /// the initial data has been loaded (dim x, dim y, storing path, origin path, and number of cores)
    START,
    /// the program has split and save all the images, number_of_pieces has been updated
    IMAGE_SPLITTED,
    /// the program has processed the images and has saved the metadata (aka the coordinates of the 4 points)
    CORNER_PROCESSED,
    /// the program has calculated all the possible connections and saved them
    CONNECTION_CALCULATED,
    /// the program has calculated, and saved the best possible combination
    COMBINATION_CALCULATED
} State;

class PuzzleSolver{
private:
    /// the final resolution of the puzzle piece i am searching
    unsigned int final_dim_x;
    unsigned int final_dim_y;
    /// the total number of input pieces
    unsigned int number_of_pieces;
    /// the path where to store files
    std::string storing_path;
    /// the path where the original data are saved (the non split images)
    std::string origin_path;
    /// the maximum number of cores the program will be using
    unsigned int number_of_cores;
    /// the current state of the solver
    State state;
};


#endif //PROCESSING_PUZZLESOLVER_H
