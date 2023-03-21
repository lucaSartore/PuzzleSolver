#include <iostream>
#include "piece_splitting.h"
#include "corner_finding.h"

using namespace std;
using namespace cv;


// on my 5700u laptop this function can process around 110 images/minute
int main(){

    int number_of_pieces;
    number_of_pieces = split_pieces_into_single_images("../blue_500pcs/raw","../blue_500pcs/divided",1200, false);

    //cout << "i have found " << number_of_pieces << " puzzle pieces" << endl;

    do_pre_processing("../blue_500pcs/divided",number_of_pieces, 1200, true, false);

    cout << "preprocessing finished" << endl;

    return 0;
}

