//
// Created by luca on 3/17/23.
//

#include "corner_finding.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include "util.h"
#include "piece_splitting.h"
#include "corner_finding.h"
using namespace std;
using namespace cv;



void find_corner(const std::string& path,const int number_of_pieces,const int ppi,const bool use_multithreading,const bool enable_image_view){

    // impossible to enable the view if multi threading is in use!
    if(use_multithreading){
        assert(enable_image_view == false);
    }

    if(use_multithreading){

    }else{
        for(int i = 1; i<=number_of_pieces; i++){
            find_corner_thread(path,i,ppi,enable_image_view);
        }
    }
}

void find_corner_thread(const std::string& path,const int piece_index,const int ppi,const bool enable_image_view){

    // load the piece
    string piece_path = path + string("/") +  to_string(piece_index) + string(".jpeg");
    Mat piece = imread(piece_path, IMREAD_GRAYSCALE);
    assert(! piece.empty());

    // removing the hole of the piece
    Mat piece_no_holes;
    remove_holes(piece,piece_no_holes);

    show(piece_no_holes);
}


// this function remove the holes of the puzzle in the following way:
// - the algorithm find the filler of a piece.
//         a filler is an area that need to be included for the image to be considered convex
// - i erode them by FIRST_EROSION_KERNEL_SIZE and expand them by FIRST_EROSION_KERNEL_SIZE + 1 to remove tiny lines
// - then i split all the mask into all the separated pieces, and consider them only if their area in pixels is grater
//   then `FILLER_AREA_THRESHOLD`
// - i erode the area with a kernel of `SECOND_EROSION_KERNEL_SIZE` so that the small lines (like the one close to a hole in
//   the puzzle get removed
// at this point i'm left with 2 kind of "filler" the formed inside the hole of a piece, and the one formed between a corner
// of a piece and a close knob.
// these 2 forms are quite different, one resembles a triangle, the other one a circle. so to distinguish them:
// - i erode the convex hull of the piece using a specific kernel size. using a binary search i find the minimum kernel
//   size for wick the erosion in AND with the piece give an empty mask.
// - i erode the convex hull by half the kernel size found in the previous step.
// - if the shape is similar to a circle the area will be about an half
// - if the shape is similar to a triangle the area will be about a quarter.
// - using the `MIN_SHRINKING_PERCENTAGE` constant i split the 2 categories into triangle or circle
// - only if is a circle i fill the area up in the original mask since it was an hole
#define FILLER_AREA_THRESHOLD (9000*ppi/1200)
#define FIRST_EROSION_KERNEL_SIZE (6*ppi/1200)
#define SECOND_EROSION_KERNEL_SIZE (100*ppi/1200)
#define MIN_SHRINKING_PERCENTAGE  0.44
///this function removes the "holes" of the puzzle, so that it could be processed easily later
void remove_holes(const Mat &input, Mat &output,const int ppi){
    Mat temp;
    Mat temp2;
    Mat kernel;

    // check color depth
    assert(input.type() == CV_8U);
    // check is grayscale
    assert(input.channels() == 1);

    // copy image
    Mat piece = input;

    // find the convex hull of the original mask
    Mat original_convex_hull;
    quick_convex_hull(piece,original_convex_hull);

    // the filler pieces that can make the original image a convex shape
    Mat filler;
    bitwise_xor(original_convex_hull,piece,filler);

    //erode and expand the border of the image to remove thin line
    Mat filler_eroded;
    kernel = Mat::zeros(Size(FIRST_EROSION_KERNEL_SIZE,FIRST_EROSION_KERNEL_SIZE),CV_8U) == 0;
    erode(filler,filler_eroded,kernel);
    Mat kernel2 = Mat::zeros(Size(FIRST_EROSION_KERNEL_SIZE+1,FIRST_EROSION_KERNEL_SIZE+1),CV_8U) == 0;
    dilate(filler_eroded,filler,kernel2);


    // split the filler into single piece
    Mat filler_split;
    int number_of_pieces = connectedComponents(filler,filler_split);

    // a copy of the original piece, in this one we will remove the holes!
    Mat piece_no_hole = piece.clone();

    // decide if every piece is a hole or not (using the strategy described above), if is a hole i fill it up
    for (int i=1; i<number_of_pieces; ++i){

        // a filler taken individually, and tho in the mask
        Mat single_filler = filler_split == i;


        Mat convex_hull = original_convex_hull.clone();

        // do a first erosion, to remove the thin area close to the puzzle hole
        kernel = Mat::zeros(Size(SECOND_EROSION_KERNEL_SIZE, SECOND_EROSION_KERNEL_SIZE), CV_8U) == 0;
        erode(convex_hull, temp, kernel);
        convex_hull = temp;

        // check that the filler is not just a thinly line due to a broder imperfection
        Mat remaining_filler;
        bitwise_and(convex_hull, single_filler, remaining_filler);

        // as described above, i consider it a filler only if is of a certain size
        if(countNonZero(single_filler) > FILLER_AREA_THRESHOLD && countNonZero(remaining_filler) > 0){
            // calculating the distance from the most distance pixel in a filler and the border
            int last_zero_found = INT_MAX;
            int distance = 100;
            int distance_prev = 0;
            int last_non_zero_found = 0;
            // binary search loop
            while (distance != distance_prev){
                distance_prev = distance;

                // eroding the convex hull
                Mat convex_hull_erosion_temp;
                kernel = Mat::zeros(Size(distance,distance),CV_8U) == 0;
                erode(convex_hull, convex_hull_erosion_temp, kernel);

                // mask containing the pixels at a distance grater then `distance`
                Mat masked_filler;
                bitwise_and(convex_hull_erosion_temp,single_filler,masked_filler);

                // binary searcher
                if (countNonZero(masked_filler) > 0){
                    // distance is to small
                    last_non_zero_found = distance;
                    if(last_zero_found == INT_MAX){
                        distance*=2;
                    }else{
                        distance = (distance+last_zero_found)/2;
                    }
                }else{
                    // distance is too big
                    last_zero_found = distance;
                    distance = (distance + last_non_zero_found)/2;
                }
            }

            // an erosion with the kernel that is half of the calculated distance
            Mat convex_hull_half_erosion;
            kernel = Mat::zeros(Size(distance/2,distance/2),CV_8U) == 0;
            erode(convex_hull, convex_hull_half_erosion, kernel);

            // now calculate the areas of the filler with a minor erosion, an the one of the filler with hte eorision alf of his size
            Mat single_filler_erosion_1, single_filler_erosion_2;
            bitwise_and(single_filler, convex_hull, single_filler_erosion_1);
            bitwise_and(single_filler,convex_hull_half_erosion,single_filler_erosion_2);

            // as described above i calculate bt how match the mouth has shrunk
            float shrinking_percentage = (float)countNonZero(single_filler_erosion_2)/ (float)countNonZero(single_filler_erosion_1);

            if(shrinking_percentage > MIN_SHRINKING_PERCENTAGE){
                bitwise_or(piece_no_hole,single_filler,temp);
                piece_no_hole = temp;
                temp = Mat();
            }
        }
    }
    output = piece_no_hole;
}


// this function removes the knobs, it dose so with the following steps:
// - it apply an erosion followed by an expansion with size `EROSION_AND_EXPANSION_SIZE`
//   in this way the knob will be removed and the angle wll be smoothed out leaving us with a square
//   that more or less resemble the original piece
// - i then create a mask with the pixels that were white in the original mask, but not in the eroded and expanded one...
//   this pixels will include the knobs and the angle of the piece
// - i apply a second erosion with size `KNOB_EROSION_SIZE` in order to remove small lines that might be created
// - i then split the mask into all is subcomponent, an consider only the one with area greater then `MINIMUM_KNOB_AREA`
//   in this way i obtain only the knobs, since angles and other things are too small
// - when i know an area is a knob i calculate is center, and remove all the white pixels in a radius of `KNOB_REMOVER_RADIUS`
//   from the center, in this way i have removed the knob
//
//  the operations of erosion and expansion are really heavy, so the image get resized to a lower resolution using the
//  `RESIZE_DIVISION_FACTOR` coefficient
#define EROSION_AND_EXPANSION_SIZE 75 // don't need to adjust for PPI since the division factor is already adjusted
#define RESIZE_DIVISION_FACTOR (6*ppi/1200)
#define KNOB_EROSION_SIZE 15 // don't need to adjust for PPI since the division factor is already adjusted
#define MINIMUM_KNOB_AREA (15000*ppi/1200)
#define KNOB_REMOVER_RADIUS (250*ppi/1200)
/// this function take as input an image (that has to have the oles already removed) and remove the knobs
void remove_knobs(const cv::Mat&input, cv::Mat &output,const int ppi){

    Mat piece = input, kernel,temp;

    // resize the image to low resolution to make the process faster (since we don't need too muck precision)
    Mat piece_resize;
    resize(piece,piece_resize,piece.size()/RESIZE_DIVISION_FACTOR);

    // just to be sure that there are no tiny dost inside the original mask...
    floodFill(piece_resize,Point(0,0),Scalar(100));
    piece_resize = piece_resize != 100;

    // creating kernel for erosion and expansion
    kernel = Mat::zeros(Size(EROSION_AND_EXPANSION_SIZE, EROSION_AND_EXPANSION_SIZE), CV_8U);
    circle(kernel,Point(EROSION_AND_EXPANSION_SIZE/2,EROSION_AND_EXPANSION_SIZE/2),(EROSION_AND_EXPANSION_SIZE-3)/2,Scalar(255),-1);

    // eroding and expansion the mask remove the "bumps" along the corners,
    Mat piece_with_smooth_corner;
    erode(piece_resize,temp,kernel);
    dilate(temp,piece_with_smooth_corner,kernel);

    // bumps_along_corner = piece_resize AND ( NOT piece_with_smooth_corner)
    // this mask now contains the bumps, and the angles of the original image
    Mat bumps_along_corner;
    temp = piece_with_smooth_corner == 0;
    bitwise_and(piece_resize,temp,bumps_along_corner);

    // eroding the `bumps_along_corner_eroded` make the connection piece more distinguishable from the corner of the piece
    kernel = Mat::zeros(Size(KNOB_EROSION_SIZE, KNOB_EROSION_SIZE), CV_8U);
    circle(kernel, Point(KNOB_EROSION_SIZE / 2, KNOB_EROSION_SIZE / 2), (KNOB_EROSION_SIZE - 3) / 2, Scalar(255), -1);
    Mat bumps_along_corner_eroded;
    erode(bumps_along_corner,bumps_along_corner_eroded,kernel);

    // resize the image
    Mat knobs_and_angles;
    resize(bumps_along_corner_eroded,knobs_and_angles,piece.size());

    // maks where i will remove the knobs
    Mat piece_with_no_knobs = piece.clone();

    // split
    Mat individual_knobs, stats, center;
    int number_of_pieces = connectedComponentsWithStats(knobs_and_angles,individual_knobs,stats,center);

    for(int i=1; i<number_of_pieces; i++){
        // find the individual knob
        Mat knob = individual_knobs == i;

        // check that he is actually a knob and not just an angle
        if(countNonZero(knob) > MINIMUM_KNOB_AREA){
            // if it is an actual knob i remove an area from the original image

            //calculate center of the knob
            int c_y = stats.at<int>(i, cv::CC_STAT_TOP) + stats.at<int>(i, cv::CC_STAT_HEIGHT)/2;
            int c_x = stats.at<int>(i, cv::CC_STAT_LEFT) + stats.at<int>(i, cv::CC_STAT_WIDTH)/2;

            circle(piece_with_no_knobs,Point(c_x,c_y),KNOB_REMOVER_RADIUS,Scalar(0),-1);
        }
    }

    output = piece_with_no_knobs;
};