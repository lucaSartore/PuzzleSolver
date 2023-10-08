// this is the command used to have access to the puzzle solver's functionalities from the terminal
#include <iostream>
#include "PuzzleSolver.h"
using namespace std;


// CommandExample
// PuzzleSolver "input_file_path" "output_file_path" "threshold_to_use" "puzzle_height" "puzzle_width" "number_of_cores"
int main(int argc, char ** argv){

    if(argc != 7){
        cout << "Wrong number of arguments, expected 6 got " << argc-1 << endl;
        cout << "The command must be in the following format:" << endl;
        cout << "\t" << "PuzzleSolver \"input_file_path\" \"output_file_path\" \"threshold_to_use\" \"puzzle_height\" \"puzzle_width\" \"number_of_cores\"" << endl;
        return -1;
    }

    char* input_path = argv[1];
    char* output_path = argv[2];

    int threshold = std::strtol(argv[3],NULL,10);
    int height = std::strtol(argv[4],NULL,10);
    int width = std::strtol(argv[5],NULL,10);
    int cores = std::strtol(argv[6],NULL,10);

    if (threshold < 0 || threshold > 255){
        cout << "threshold must be between 0 and 255" << endl;
        return -1;
    }

    if (height < 0){
        cout << "height must be positive" << endl;
        return -1;
    }

    if (width < 0){
        cout << "width must be positive" << endl;
        return -1;
    }

    if (cores < 0) {
        cout << "number of cores must be positive" << endl;
        return -1;
    }

    cout << "You have selected: " << endl;
    cout << "\t input_path: " << input_path << endl;
    cout << "\t output_path: " << output_path << endl;
    cout << "\t threshold: " << threshold << endl;
    cout << "\t height: " << height << endl;
    cout << "\t width: " << width << endl;
    cout << "\t number_of_cores: " << cores << endl;

    PuzzleSolver ps(width, height, output_path,input_path,cores);

    ps.set_threshold(threshold);

    ps.split_image();
    ps.process_corners();
    ps.calculate_connections();
    ps.solve_puzzle();

    return 0;
}