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
#define DIRECTORY "blue_500pcs"
#define IMAGE_FORMAT ".jpg"
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
        break;
    }

    // step 1: converting all the images to gray
    vector<Mat> mask_images = vector<Mat>();
    for(auto raw_image: raw_images){
        // converting image to gray
        Mat gray_image;
        cvtColor(raw_image, gray_image, COLOR_BGR2GRAY);
        // applying a threshold
        Mat threshold_image;
        threshold(gray_image, threshold_image, 25, 255, THRESH_BINARY);


        // create a matrix kernel full of 255
        int kernel_data[20*20];
        for(auto &i: kernel_data){i=255;}
        Mat kernel = Mat(20,20,CV_8U, kernel_data);

        // remove small white dots
        Mat white_dots_removed_image;
        morphologyEx(threshold_image,white_dots_removed_image,MORPH_OPEN,kernel);

        // create a matrix kernel full of 255
        int kernel_data2[12*12];
        for(auto &i: kernel_data){i=255;}
        Mat kernel2 = Mat(12,12,CV_8U, kernel_data2);

        // remove black white dots
        Mat black_dots_removed_image;
        morphologyEx(white_dots_removed_image,black_dots_removed_image,MORPH_CLOSE,kernel2);

        mask_images.push_back(black_dots_removed_image);
    }
    //Mat resized;
    //resize(mask_images[0],resized,Size(70,100));

    Mat individual_pieces;
    int number_of_pieces = connectedComponents(mask_images[0],individual_pieces);

    cout << individual_pieces.depth();

    Mat new_dype;
    individual_pieces.convertTo(new_dype,CV_8U);


    Mat resized;
    resize(new_dype,resized,Size(700,1000));


    Mat test = resized == 22;

    imshow( "Connected Components", test);
    waitKey(0);



    return 0;
}