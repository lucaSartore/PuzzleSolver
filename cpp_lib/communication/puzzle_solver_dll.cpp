
#include "puzzle_solver_dll.h"
#include "../PuzzleSolver.h"
__declspec(dllexport)void* open_folder(const char *folder_path){
    try{
        return (void*) new PuzzleSolver(folder_path);
    }catch(std::exception &e){
        return nullptr;
    }
}
__declspec(dllexport) void free_memory(void* puzzle_solver){
    delete (PuzzleSolver*) puzzle_solver;
}

__declspec(dllexport) void* create_new(unsigned int dim_x, unsigned int dim_y, const char* work_path, const char* origin_path, int number_of_cores){
    try{
        return (void*) new PuzzleSolver(dim_x,dim_y,work_path,origin_path,number_of_cores);
    }catch(std::exception &e){
        return nullptr;
    }
}

__declspec(dllexport) void set_split_threshold(void* puzzle_solver, int new_threshold){
     ((PuzzleSolver*)puzzle_solver)->set_threshold(new_threshold);
}

/// return an image that show a binary representation of the piece, to tweak the threshold
__declspec(dllexport) PngImagePointer get_threshold_preview(void* puzzle_solver){
    try{
        return ((PuzzleSolver*)puzzle_solver)->get_test_threshold_image();
    }catch (std::exception &e){
        return PngImagePointer(nullptr,0);
    }
}


/// get the curren state of the program
__declspec(dllexport) int get_state(void* puzzle_solver){
    return (int)((PuzzleSolver*)puzzle_solver)->get_state();
}

/// split the images into individual pieces, and update `number_of_pieces`
/// return the number of peaces found
__declspec(dllexport) int split_image(void* puzzle_solver){
    return ((PuzzleSolver*)puzzle_solver)->split_image();
}

/// take the splitted images and calculate the corners of each of them
__declspec(dllexport) int process_corners(void* puzzle_solver){
    ((PuzzleSolver*)puzzle_solver)->process_corners();
    return 0;
}

/// calculate all the possible connections and save the results
__declspec(dllexport) int calculate_connections(void* puzzle_solver){
    ((PuzzleSolver*)puzzle_solver)->calculate_connections();
    return 0;
}

/// run the solving algorithm and find the best possible combination
/// return -1 if it dose not find a solution, 0 if he finds one
__declspec(dllexport) int solve_puzzle(void* puzzle_solver){
    ((PuzzleSolver*)puzzle_solver)->solve_puzzle();
    return 0;
}

/// enable the preview
__declspec(dllexport) void enable_preview(){
    PreviewManager::enable_preview();
}

/// disable the preview
__declspec(dllexport) void disable_preview(){
    PreviewManager::disable_preview();
}

/// lock the program until the preview image is ready or until the time max_wait_time in seconds has passed, and return an image
__declspec(dllexport) PngImagePointer next_preview_image(float max_wait_time){

    bool res = PreviewManager::next_preview_image(max_wait_time);

    // if no image has been generated
    if(res == false){
        // return an empty image
        return PngImagePointer(nullptr,0);
    }

    return PreviewManager::get_image_binary();
}

