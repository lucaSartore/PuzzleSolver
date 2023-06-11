#include <iostream>
#include "piece_splitting.h"
#include "corner_finding.h"

using namespace std;
using namespace cv;


// on my 5700u laptop this function can process around 110 images/minute
int main(){

    string path = "../../../dataset/test_4x4";

    int number_of_pieces = 16;
    //number_of_pieces = split_pieces_into_single_images(path + "/raw",path + "/divided",1200, true);

    cout << "i have found " << number_of_pieces << " puzzle_preview pieces" << endl;

    find_corners(path+"/divided",number_of_pieces, 1200, 1, true);

    cout << "preprocessing finished" << endl;

    return 0;
}

