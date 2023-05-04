//
// Created by luca on 5/3/23.
//

#include <iostream>
#include "PuzzleSolver.h"

using namespace std;


int main(){

    PuzzleSolver ps(4, 4, "../work_path", "../dataset/test_4x4/raw",2);
    //PuzzleSolver ps("../work_path");


    cout << ps << endl;


    ps.split_image();

    ps.process_corners();


    return 0;

}