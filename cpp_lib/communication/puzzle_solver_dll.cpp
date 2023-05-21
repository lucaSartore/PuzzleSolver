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

