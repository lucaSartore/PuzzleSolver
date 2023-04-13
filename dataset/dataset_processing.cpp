#include <iostream>
#include "piece_splitting.h"
#include "corner_finding.h"

using namespace std;
using namespace cv;


// on my 5700u laptop this function can process around 110 images/minute
int main(){

    int number_of_pieces = 500;
    number_of_pieces = split_pieces_into_single_images("../test_2x3/raw","../test_2x3/divided",1200, true);

    cout << "i have found " << number_of_pieces << " puzzle pieces" << endl;

    do_pre_processing("../test_2x3/divided",number_of_pieces, 1200, false, true);

    cout << "preprocessing finished" << endl;

    return 0;
}

