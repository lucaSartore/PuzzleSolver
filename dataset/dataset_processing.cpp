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
// format of the file
#define IMAGE_FORMAT ".jpg"
// how many pixels a piece's area has to be big for it to be considered an actual piece
#define PIECE_MINIMUM_AREA 50000


void split_pieces_into_single_images();

int main(){

    split_pieces_into_single_images();


}

void split_pieces_into_single_images(){
    // index for keeping tracking the scanned files
    int raw_image_index = 1;
    // index for keeping tracking the files to write
    int piece_index = 1;

    while (true){


        //step 1: read all the files
        string path = string("../") + string(DIRECTORY) + string("/raw/") + to_string(raw_image_index) + string(IMAGE_FORMAT);
        // image with the scanned raw data
        Mat raw_image = imread(path);
        // break in the case the image is empty
        if (raw_image.empty()){
            if(raw_image_index == 1){
                cerr << "no file found with name: " << path << endl;
                exit(1);
            }else{
                cout << "total file read: " << raw_image_index << endl;
                break;
            }
        }
        raw_image_index++;

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
        Mat final_mask;
        morphologyEx(white_dots_removed_image,final_mask,MORPH_CLOSE,kernel2);

        // split the pieces and get the data
        Mat individual_pieces, stats, center;
        int number_of_pieces = connectedComponentsWithStats(final_mask,individual_pieces,stats,center);

        // for each individual piece
        for(int i=1; i<=number_of_pieces; i++){
            // coordinates to crop the image
            int x1 = -1 + stats.at<int>(i, cv::CC_STAT_LEFT);
            int y1 = -1 + stats.at<int>(i, cv::CC_STAT_TOP);
            int x2 = 1 + x1 + stats.at<int>(i, cv::CC_STAT_WIDTH);
            int y2 = 1 + y1 + stats.at<int>(i, cv::CC_STAT_HEIGHT);
            // area to understand if the considered mask is actually a piece of puzzle or just a tiny dot
            int area = stats.at<int>(i  , cv::CC_STAT_AREA);

            // the piece has to be big enough
            if(area > PIECE_MINIMUM_AREA){
                cout << area << endl;
                // get the individual piece
                Mat single_piece_32U = individual_pieces == i;
                Mat single_piece;
                // convert it to simpler format
                single_piece_32U.convertTo(single_piece,CV_8U);
                // crop it
                Mat cropped_image = single_piece(Range(y1,y2),Range(x1,x2));

                //show the piece corpped
                Mat cropped_image_resized;
                Mat to_target_resized;
                resize(cropped_image,cropped_image_resized,Size(400,400));
                Mat to_target = raw_image;
                rectangle(to_target,Point(x1,y1),Point(x2,y2),Scalar(255,0,0,0),30);
                resize(to_target,to_target_resized,Size(550,800));
                imshow( "output", cropped_image_resized);
                imshow( "where", to_target_resized);
                waitKey(0);

                //calculate path
                string path = string("../") + string(DIRECTORY) + string("/deviated/") + to_string(piece_index++) + string(IMAGE_FORMAT);

                //save the file
                imwrite(path,cropped_image);
            }
        }
    }
}