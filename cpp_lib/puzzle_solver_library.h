#ifndef PUZZLESOLVER_PUZZLE_SOLVER_LIBRARY_H
#define PUZZLESOLVER_PUZZLE_SOLVER_LIBRARY_H

#include <iostream>

extern "C" {
    int __declspec(dllexport) create_puzzle_solver(int dim_x, int dim_y, char *work_path, char *input_path, int number_of_cores);

    int __declspec(dllexport) split_image(const char *work_path);

    int __declspec(dllexport) process_corners(const char *work_path);

    int __declspec(dllexport) calculate_connections(const char *work_path);

    int __declspec(dllexport) solve_puzzle(const char *work_path);
}


#endif //PUZZLESOLVER_PUZZLE_SOLVER_LIBRARY_H
