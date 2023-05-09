#ifndef PUZZLESOLVER_PUZZLE_SOLVER_LIBRARY_H
#define PUZZLESOLVER_PUZZLE_SOLVER_LIBRARY_H

#include <iostream>


int create_puzzle_solver(int dim_x, int dim_y, std::string input_path, int number_of_cores);

int split_image(std::string work_path);

int process_corners(std::string work_path);

int calculate_connections(std::string work_path);

int solve_puzzle(std::string work_path);


#endif //PUZZLESOLVER_PUZZLE_SOLVER_LIBRARY_H
