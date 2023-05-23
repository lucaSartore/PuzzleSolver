
#include "puzzle_solver_dll.h"
#include "../PuzzleSolver.h"
void* __declspec(dllexport) open_folder(const char *folder_path){
    try{
        return (void*) new PuzzleSolver(folder_path);
    }catch(std::exception &e){
        return nullptr;
    }
}
void __declspec(dllexport) free_memory(void* puzzle_solver){
    delete (PuzzleSolver*) puzzle_solver;
}

void* __declspec(dllexport) create_new(unsigned int dim_x, unsigned int dim_y, const char* work_path, const char* origin_path, int number_of_cores){
    try{
        return (void*) new PuzzleSolver(dim_x,dim_y,work_path,origin_path,number_of_cores);
    }catch(std::exception &e){
        return nullptr;
    }
}

void __declspec(dllexport) set_split_threshold(void* puzzle_solver, unsigned char new_threshold){
     ((PuzzleSolver*)puzzle_solver)->set_threshold(new_threshold);
}

/// return an image that show a binary representation of the piece, to tweak the threshold
PngImagePointer __declspec(dllexport) get_threshold_preview(void* puzzle_solver){
    try{
        return ((PuzzleSolver*)puzzle_solver)->get_test_threshold_image();
    }catch (std::exception &e){
        return PngImagePointer(nullptr,0);
    }
}


/// get the curren state of the program
int __declspec(dllexport) get_state(void* puzzle_solver){
    return (int)((PuzzleSolver*)puzzle_solver)->get_state();
}

/// split the images into individual pieces, and update `number_of_pieces`
/// return the number of peaces found
int __declspec(dllexport) split_image(void* puzzle_solver){
    return ((PuzzleSolver*)puzzle_solver)->split_image();
}

/// take the splitted images and calculate the corners of each of them
int __declspec(dllexport) process_corners(void* puzzle_solver){
    ((PuzzleSolver*)puzzle_solver)->process_corners();
    return 0;
}

/// calculate all the possible connections and save the results
int __declspec(dllexport) calculate_connections(void* puzzle_solver){
    ((PuzzleSolver*)puzzle_solver)->calculate_connections();
    return 0;
}

/// run the solving algorithm and find the best possible combination
/// return -1 if it dose not find a solution, 0 if he finds one
int __declspec(dllexport) solve_puzzle(void* puzzle_solver){
    ((PuzzleSolver*)puzzle_solver)->solve_puzzle();
    return 0;
}
