//
// Created by luca on 5/3/23.
//

#ifndef PROCESSING_PUZZLESOLVER_H
#define PROCESSING_PUZZLESOLVER_H

#include <iostream>
#include <exception>
#include <iostream>
#include "solving/puzzle_preview/PieceImage.h"
#include "solving/puzzle_preview/PieceArray.h"

/// this exception os thrown if the state machine fail to create/read/write a file during the execution
class file_system_exception: std::exception{};
/// this exception is thrown if the user call a function that is not callable in the current state of the program
class wrong_state_exception: std::exception{};


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

std::ostream& operator<<(std::ostream& os, const State& state);



class PuzzleSolver{
private:
    /// the final resolution of the puzzle piece i am searching
    unsigned int final_dim_x;
    unsigned int final_dim_y;
    /// the total number of input pieces
    unsigned int number_of_pieces;
    /// the path where to store files
    std::string work_path;
    /// the path where the original data are saved (the non split images)
    std::string origin_path;
    /// the maximum number of cores the program will be using
    unsigned int number_of_cores;
    /// the current state of the solver
    State state;
    /// create a file in `work_path` with the name status.txt and save all the parameters of the class in it
    void save_status();
    /// load the status form the file that has been saved from the save_status() function
    void load_status(std::string file);
    friend std::ostream& operator<<(std::ostream& os, const PuzzleSolver& ps);
public:
    /// create a new instance of the solver
    /// Arguments:
    ///  - dim_x: the x dimension of the puzzle you want to solve
    ///  - dim_y: the y dimension of the puzzle you want to solve
    ///  - work_path: a folder where the program can work, and save his files
    ///  - origin_path: a folder containing ONLY the images obtained by scanning the puzzle
    ///  - number_of_cores: the cpu cores the program will use at most, default: 1
    PuzzleSolver(int dim_x, int dim_y, std::string work_path_, std::string origin_pat_, int number_of_cores_ = 1);
    /// create an instance of the solver, by reading the data found in work_path_ and restarting from there
    explicit PuzzleSolver(std::string work_path_);
    /// split the images into individual pieces, and update `number_of_pieces`
    void split_image();
    /// take the splitted images and calculate the corners of each of them
    void process_corners();
    /// calculate all the possible connections and save the results
    void calculate_connections();
    /// run the solving algorithm and find the best possible combination
    void solve_puzzle();
    /// destructor
    ~PuzzleSolver();
};


std::ostream& operator<<(std::ostream& os, const PuzzleSolver& ps);

#endif //PROCESSING_PUZZLESOLVER_H
