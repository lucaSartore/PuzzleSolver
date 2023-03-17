//
// Created by luca on 3/16/23.
//

#include "piece_splitting.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include "util.h"

using namespace std;
using namespace cv;

/// how many pixels a piece's area has to be big for it to be considered an actual piece
#define PIECE_MINIMUM_AREA (50000*ppi/1200)
/// how many pixel of margin the function keeps when cropping an image
#define CROP_MARGIN (50*ppi/1200)
/// the threshold that gets applied to the original mask, in order to split the pieces from the background
#define THRESHOLD 25
/// kernel of the morphologyEx open filter that will be apply to the mask
#define MORPH_OPEN_KERNEL (10*ppi/1200)
/// kernel of the morphologyEx open filter that will be apply to the mask
#define MORPH_CLOSED_KERNEL (12*ppi/1200)

/// this function take as input a input_path where some scansion of a puzzle is made
/// and split them into many single pieces
void split_pieces_into_single_images(const std::string& input_path,const std::string& output_path,const int ppi, bool enable_image_view){

    // a temporary value to to quick operations
    Mat temp, kernel;
    // index for keeping tracking the files to write
    int piece_index = 1;

    for (const auto & entry : std::filesystem::directory_iterator(input_path)){

        //step 1: read the files one by one;
        Mat image = imread(entry.path());

        // expand the image border so that it could crop the pieces without "overflowing" and throwing an error
        int width = image.size().width, height = image.size().height;
        Size new_size = Size(width + 2*CROP_MARGIN,height + 2*CROP_MARGIN);
        temp = Mat::zeros(new_size,image.type());
        cv::Mat middle_image = Mat(temp, cv::Rect(CROP_MARGIN, CROP_MARGIN, width,height));
        image.copyTo(middle_image);
        image = temp;

        Mat mask = image.clone();


        // converting mask to gray
        cvtColor(mask, temp, COLOR_BGR2GRAY);
        mask = temp;

        // applying a threshold
        threshold(mask, temp, THRESHOLD, 255, THRESH_BINARY);
        mask = temp;

        // create a matrix kernel for the filter
        kernel = Mat::zeros(Size(MORPH_OPEN_KERNEL,MORPH_OPEN_KERNEL),CV_8U) == 0;

        // remove small white dots
        morphologyEx(mask, temp, MORPH_OPEN, kernel);
        mask = temp;

        // create a matrix kernel for the filter
        kernel = Mat::zeros(Size(MORPH_CLOSED_KERNEL,MORPH_CLOSED_KERNEL),CV_8U) == 0;

        // remove black white dots
        morphologyEx(mask, temp, MORPH_CLOSE, kernel);
        mask = temp;

        // split the pieces and get the data
        Mat individual_pieces, stats, center;
        int number_of_pieces = connectedComponentsWithStats(mask, individual_pieces, stats, center);

        // for each individual piece
        for(int i=1; i<number_of_pieces; i++){

            // coordinates to crop the mask
            int x1 = -CROP_MARGIN + stats.at<int>(i, cv::CC_STAT_LEFT);
            int y1 = -CROP_MARGIN + stats.at<int>(i, cv::CC_STAT_TOP);
            int x2 = CROP_MARGIN*2 + x1 + stats.at<int>(i, cv::CC_STAT_WIDTH);
            int y2 = CROP_MARGIN*2 + y1 + stats.at<int>(i, cv::CC_STAT_HEIGHT);
            // area to understand if the considered mask is actually a piece of puzzle or just a tiny dot
            int area = stats.at<int>(i  , cv::CC_STAT_AREA);

            // the piece has to be big enough
            if(area > PIECE_MINIMUM_AREA){

                // show progress
                cout << "processing piece: " << piece_index  <<endl;

                // get the individual piece
                Mat single_piece = individual_pieces == i;

                // convert it to 8 bit format
                single_piece.convertTo(temp,CV_8U);
                single_piece = temp;

                // crop it
                Mat cropped_image = single_piece(Range(y1,y2),Range(x1,x2));

                //show the piece corpped
                if(enable_image_view){
                    Mat cropped_image_resized;
                    Mat to_target_resized;
                    resize(cropped_image, cropped_image_resized, Size(400, 400));
                    Mat to_target = image;
                    rectangle(to_target, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0, 0), 30);
                    resize(to_target, to_target_resized, Size(550, 800));
                    imshow("output", cropped_image_resized);
                    imshow("where", to_target_resized);
                    waitKey(0);
                }


                //calculate input_path
                string output_path_image = output_path + string("/") + to_string(piece_index++) + string(".jpeg");

                //make share that there are not white pixels in the border by creating a rectangle
                int x = cropped_image.size().width, y = cropped_image.size().height;
                rectangle(cropped_image,Point(0,0),Point(x-1,y-1),Scalar(0),CROP_MARGIN*2-4);


                //save the file
                imwrite(output_path_image,cropped_image);
            }
        }
    }
}
