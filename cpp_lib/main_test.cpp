//
// Created by luca on 5/3/23.
//

#include <iostream>
#include "PuzzleSolver.h"
#include <thread>
#include <opencv2/opencv.hpp>

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

    PuzzleSolver ps(4, 4, "../work_path", "../../dataset/test_2x3/raw",6);
    //PuzzleSolver ps("../work_path");


    ps.set_threshold(50);

    cout << ps << endl;

    Mat img = load_image_from_ram(ps.get_test_threshold_image());
    Mat temp;
    resize(img,temp,img.size()/10);
    imshow("A",temp);
    waitKey(0);

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
    t1 = thread(debug_thread);

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