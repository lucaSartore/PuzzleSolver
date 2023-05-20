
#include "PuzzleSolver.h"
#include "puzzle_solver_library.h"

#include <utility>
#include "exception"
int create_puzzle_solver(int dim_x, int dim_y,char* work_path, char* input_path, int number_of_cores){
    try{
        PuzzleSolver ps(dim_x, dim_y, work_path, input_path,number_of_cores);
        return 0;
    }catch(std::exception &exception){
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}

int split_image(const char* work_path){
    try{
        PuzzleSolver ps(work_path);
        ps.split_image();
        return 0;
    }catch(std::exception &exception){
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}

int process_corners(const char* work_path){
    try{
        PuzzleSolver ps(work_path);
        ps.process_corners();
        return 0;
    }catch(std::exception &exception){
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}

int calculate_connections(const char* work_path){
    try{
        PuzzleSolver ps(work_path);
        ps.calculate_connections();
        return 0;
    }catch(std::exception &exception){
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}

int solve_puzzle(const char* work_path){
    try{
        PuzzleSolver ps(work_path);
        ps.solve_puzzle();
        return 0;
    }catch(std::exception &exception){
        std::cerr << exception.what() << std::endl;
        return 1;
    }
}
