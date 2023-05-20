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