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
// how many pixel of margin the crop keeps
#define CROP_MARGIN 10

void split_pieces_into_single_images();

void do_images_pre_processing();

void quick_convex_hull(Mat &input, Mat &output_);

int main(){

    //split_pieces_into_single_images();

    do_images_pre_processing();

    return 0;

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
            int x1 = -CROP_MARGIN + stats.at<int>(i, cv::CC_STAT_LEFT);
            int y1 = -CROP_MARGIN + stats.at<int>(i, cv::CC_STAT_TOP);
            int x2 = CROP_MARGIN*2 + x1 + stats.at<int>(i, cv::CC_STAT_WIDTH);
            int y2 = CROP_MARGIN*2 + y1 + stats.at<int>(i, cv::CC_STAT_HEIGHT);
            // area to understand if the considered mask is actually a piece of puzzle or just a tiny dot
            int area = stats.at<int>(i  , cv::CC_STAT_AREA);

            // the piece has to be big enough
            if(area > PIECE_MINIMUM_AREA){
                // get the individual piece
                Mat single_piece_32U = individual_pieces == i;
                Mat single_piece;
                // convert it to simpler format
                single_piece_32U.convertTo(single_piece,CV_8U);
                // crop it
                Mat cropped_image = single_piece(Range(y1,y2),Range(x1,x2));

                //show the piece corpped
                /*Mat cropped_image_resized;
                Mat to_target_resized;
                resize(cropped_image,cropped_image_resized,Size(400,400));
                Mat to_target = raw_image;
                rectangle(to_target,Point(x1,y1),Point(x2,y2),Scalar(255,0,0,0),30);
                resize(to_target,to_target_resized,Size(550,800));
                imshow( "output", cropped_image_resized);
                imshow( "where", to_target_resized);
                waitKey(0);*/

                //calculate path
                string path = string("../") + string(DIRECTORY) + string("/divided/") + to_string(piece_index++) + string(IMAGE_FORMAT);

                //save the file
                imwrite(path,cropped_image);
            }
        }
    }
}


// the algorithm find the filler of a piece.
// a filler is an area that need to be included for the image to be considered convex
// the areas that will be calculated will be considered filler only if their area (in pixels) is greater than this threshold
#define FILLER_AREA_THRESHOLD 1000
// when a filler is found, it can be of 2 types...
// a filler created by a hole in the puzzle piece
// or a filler created by am extension of the puzzle piece
// if he is created by a hole it will be a convex shape
// i can define the "convex percentage", that is the area of the original image decided by the area of the convex hull of the image
// than i can define that a filler is a hole if his convex percentage is lower than this percentage
#define MIN_CONVEX_PERCENTAGE 0.9
//this function make some preprocessing to increase the speed of the comparison operation later
void do_images_pre_processing(){
    Mat temp;
    int piece_index = 1;

    while (true){
        //step 1: read all the files
        string path = string("../") + string(DIRECTORY) + string("/divided/") + to_string(piece_index) + string(IMAGE_FORMAT);
        // image with the scanned raw data
        Mat piece = imread(path, IMREAD_GRAYSCALE);
        // break in the case the image is empty
        if (piece.empty()){
            if(piece_index == 1){
                cerr << "no file found with name: " << path << endl;
                exit(1);
            }else{
                cout << "total file read: " << piece_index << endl;
                break;
            }
        }
        piece_index++;

        // find the convex hull of the original mask
        Mat convex_hull;
        quick_convex_hull(piece,convex_hull);

        // the filler pieces that can make the original image a convex shape
        Mat filler;
        bitwise_xor(convex_hull,piece,filler);

        //erode the border of the image to remove thin line
        Mat filler_eroded;
        Mat kernel = Mat::zeros(Size(6,6),CV_8U) == 0;
        erode(filler,filler_eroded,kernel);

        //resize(filler,temp,Size(500,500));
        //imshow("",temp);
        //waitKey(0);

        // split the filler into single piece
        Mat filler_split;
        int number_of_pieces = connectedComponents(filler_eroded,filler_split);

        // a copy of the original piece with removed holes
        Mat piece_no_hole = piece.clone();

        // decide if every piece is a hole or not, if is a hole i fill it up
        for (int i=1; i<=number_of_pieces; ++i){
            Mat single_filler = filler_split == i;

            // see comment on FILLER_AREA_THRESHOLD
            if(countNonZero(single_filler) > FILLER_AREA_THRESHOLD){
                Mat single_filler_convex_hull;
                quick_convex_hull(single_filler,single_filler_convex_hull);


                // see comment of MIN_CONVEX_PERCENTAGE for explanation
                float convex_percentage = (float)countNonZero(single_filler)/ (float)countNonZero(single_filler_convex_hull);
                if(convex_percentage < MIN_CONVEX_PERCENTAGE){
                    bitwise_or(piece_no_hole,single_filler,temp);
                    piece_no_hole = temp;
                }
            }

        }

        resize(piece_no_hole,temp,Size(500,500));
        imshow("",temp);
        waitKey(0);



    }
}


// find the convex hull of a mask, and fill it with white.
void quick_convex_hull(Mat &input, Mat &output){

    // create output_
    output =  Mat::zeros(input.size(), CV_8U );

    Mat canny;
    Canny(input, canny, 50, 200 );

    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

    vector<vector<Point>>hull( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );
    }

    for( size_t i = 0; i< contours.size(); i++ )
    {
        drawContours(output, hull, (int)i, 255 );
    }

    floodFill(output,Point(0,0),Scalar(255));
    output = output != 255;
}