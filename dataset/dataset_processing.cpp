/*
 * This program is created to process the images,
 *
 * he has to:
 *      - apply a threshold to the image
 *      - split the images into single pieces
 *      - find the corners
 *      - save the information for later
 * */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include "util.h"
#include "piece_splitting.h"
#include "corner_finding.h"
using namespace std;
using namespace cv;


// the directory the program will use as a destination
#define DIRECTORY "blue_500pcs"
// format of the file
#define IMAGE_FORMAT ".jpg"



// take a single piece WITH the holes already removed, and remove the "extensions" remaining with a square
void remove_extensions_and_save_corner_data();


int main(){

    //int number_of_pieces = split_pieces_into_single_images("../blue_500pcs/raw","../blue_500pcs/divided");

    //cout << "i have found " << number_of_pieces << " puzzle pieces" << endl;

    do_pre_processing("../blue_500pcs/divided",15, 1200,false, false);

    cout << "preprocessing finished" << endl;

    return 0;
}

