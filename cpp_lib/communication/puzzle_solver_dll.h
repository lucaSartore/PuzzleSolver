#ifndef PUZZLESOLVER_PUZZLE_SOLVER_DLL_H
#define PUZZLESOLVER_PUZZLE_SOLVER_DLL_H

#include "communication_image.h"
#include "../PuzzleSolver.h"

extern "C" {

    /// open an already saved folder, and return the pointer to the puzzle solver, you need to keep the pointer and pass it
    /// to the next functions
    __declspec(dllexport) void* open_folder(const char *folder_path);

    /// create a new instance of the solver
    /// Arguments:
    ///  - dim_x: the x dimension of the puzzle you want to solve
    ///  - dim_y: the y dimension of the puzzle you want to solve
    ///  - work_path: a folder where the program can work, and save his files
    ///  - origin_path: a folder containing ONLY the images obtained by scanning the puzzle
    ///  - number_of_cores: the cpu cores the program will use at most, default: 1
    ///
    /// return the pointer to the puzzle solver, you need to keep the pointer and pass it to the next functions
    __declspec(dllexport) void* create_new(unsigned int dim_x, unsigned int dim_y, const char* work_path, const char* origin_path, int number_of_cores);

    /// save the project in a new path, need to pass the pointer to the class and the new path
    __declspec(dllexport) void save_as(void* puzzle_solver,const char *new_path);

    /// deallocate the memory associated with the puzzle solver pointer
    __declspec(dllexport) void free_memory(void* puzzle_solver);

    /// set the threshold
    __declspec(dllexport) void set_split_threshold(void* puzzle_solver, unsigned char new_threshold);

    /// enable the preview
    __declspec(dllexport) void enable_preview();

    /// disable the preview
    __declspec(dllexport) void disable_preview();

    /// lock the program until the preview image is ready or until the time max_wait_time in seconds has passed, and return an image
    __declspec(dllexport) PngImagePointer next_preview_image(float max_wait_time);

    /// return an image that show a binary representation of the piece, to tweak the threshold
    __declspec(dllexport) PngImagePointer get_threshold_preview(void* puzzle_solver);

    /// get the curren state of the program
    __declspec(dllexport) int get_state(void* puzzle_solver);

    /// split the images into individual pieces, and update `number_of_pieces`
    /// return the number of peaces found
    __declspec(dllexport) int split_image(void* puzzle_solver);

    /// take the splitted images and calculate the corners of each of them
    __declspec(dllexport) int process_corners(void* puzzle_solver);

    /// calculate all the possible connections and save the results
    __declspec(dllexport) int calculate_connections(void* puzzle_solver);

    /// run the solving algorithm and find the best possible combination
    /// return -1 if it dose not find a solution, 0 if he finds one
    __declspec(dllexport) int solve_puzzle(void* puzzle_solver);
}


#endif //PUZZLESOLVER_PUZZLE_SOLVER_DLL_H
