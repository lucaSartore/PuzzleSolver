//
// Created by luca on 5/3/23.
//

#include <iostream>;
#include "PuzzleSolver.h"

using namespace std;


int main(){
    {
        // Create a new instance of PuzzleSolver
        PuzzleSolver ps(3, 55, "../work_path", "../origin_path");

        // Print the first instance
        std::cout << "First instance:\n" << ps << "\n";
    }

    // Create a new instance of PuzzleSolver using the work_path from the previous instance
    PuzzleSolver ps2("../work_path");

    // Print the second instance
    std::cout << "Second instance:\n" << ps2 << "\n";

    return 0;

}