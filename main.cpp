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

    try{
        ps.split_image();
    }catch(wrong_state_exception &e){
        cout << "skip split image" << endl;
    }

    try{
        ps.process_corners();
    }catch(wrong_state_exception &e){
        cout << "skip process corners" << endl;
    }



    try{
        ps.calculate_connections();
    }catch(wrong_state_exception &e){
        cout << "skip calculate connections" << endl;
    }


    return 0;

}