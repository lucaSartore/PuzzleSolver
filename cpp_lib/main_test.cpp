//
// Created by luca on 5/3/23.
//

#include <iostream>
#include "PuzzleSolver.h"
#include <thread>
#include <opencv2/opencv.hpp>
#include "pre_processing/corner_finding.h"
#include "solving/calculate_connections.h"
using namespace std;
using namespace cv;

void debug_thread(){

    PreviewManager::enable_preview();

    cout << "PreviewThreadEnable" << endl;
    while (PreviewManager::next_preview_image(150)){
        Mat image = PreviewManager::get_image();
        Mat resized;
        resize(image,resized,image.size()/2);
        imshow("preview",resized);
        waitKey(0);
    }
    destroyAllWindows();
    cout << "PreviewEnded" << endl;
}

int main(){

    // function to call if you only want to compare the pieces:
    //*
    calculate_all_connections(R"(C:\Users\lucas\CLionProjects\Puzzle_Solver\dataset\test_5x7_digital\divided)",
                              R"(C:\Users\lucas\CLionProjects\Puzzle_Solver\dataset\test_5x7_digital\connections)",
                              35,3, false);
    return 0;
    //*/

    //export_sides_result("C:\\Users\\lucas\\Desktop\\results\\results\\pieces","C:\\Users\\lucas\\Desktop\\results\\divided","C:\\Users\\lucas\\Desktop\\results\\results\\sides", 16);

    //return 0;


    cout << "Test" << endl;

    PuzzleSolver ps(4, 4, "../work_path", "../../dataset/test_4x4/raw",2);
    //PuzzleSolver ps("C:\\Users\\lucas\\Desktop\\output");
    //PuzzleSolver ps("../work_path");

    ps.set_threshold(100);

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

    thread t1;
    //t1 = thread(debug_thread);

    try{
        ps.solve_puzzle();
    }catch(wrong_state_exception &e){
        cout << "skip solve puzzle" << endl;
    }

    //imshow("lalala",ps.get_result());
    //waitKey(0);

    t1.join();

    return 0;

}