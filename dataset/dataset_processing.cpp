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
#include <list>
using namespace std;
using namespace cv;

// the directory the program will use as a destination
#define DIRECTORY "cat_500pcs"
#define IMAGE_FORMAT ".jpeg"
int main(){

    // list for keeping the files
    vector<Mat> raw_images = vector<Mat>();
    int c = 1;

    //read all the files
    while (true){
        string path = string("../") + string(DIRECTORY) + string("/raw/") + to_string(c) + string(IMAGE_FORMAT);
        Mat img = imread(path);

        // break in the case the image is empty
        if (img.empty()){
            if(c == 1){
                cerr << "no file found with name: " << path << endl;
                exit(1);
            }else{
                cout << "total file read: " << c << endl;
                break;
            }
        }
        raw_images.push_back(img);
        c++;
    }

    // step 1: converting all the images to gray
    vector<Mat> mask_images = vector<Mat>();
    for(auto raw_image: raw_images){
        Mat gray_image;
        cvtColor(raw_image, gray_image, COLOR_BGR2GRAY);
        Mat threshold_image;
        //threshold(gray_image,threshold,);
        //Canny( gray_image, threshold_image, 100, 200 );
        threshold(gray_image, threshold_image, 200, 255, THRESH_BINARY);
        mask_images.push_back(threshold_image);
    }

    imshow("test", mask_images[0]);
    waitKey(0);



    return 0;
}