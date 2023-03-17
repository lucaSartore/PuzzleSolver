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

    //find_corner("../blue_500pcs/divided",10);

    //remove_holes();

    remove_extensions_and_save_corner_data();

    return 0;
}



#define EROSION_AND_EXPANSION_SIZE 75
#define RESIZE_DIVISION_FACTOR 6
#define KNOB_EROSION_SIZE 15
#define MINIMUM_KNOB_AREA 15000
#define ANGLE_FINDING_BLUR_RADIUS 150
#define KNOB_REMOVER_RADIUS 250
#define ANGLE_MASK_FOR_TRIANGLE_CALCULATION 100
void remove_extensions_and_save_corner_data(){
    Mat temp;
    Mat temp2;
    Mat kernel;
    int piece_index = 1; // 121 474 416

    while (true) {
        cout << "processing piece: " << piece_index << endl;

        //step 1: read all the files
        string path =
                string("../") + string(DIRECTORY) + string("/hole_removed/") + to_string(piece_index) + string(IMAGE_FORMAT);
        // image with the scanned raw data
        Mat piece = imread(path, IMREAD_GRAYSCALE);
        // apply threshold jut to be shore...
        piece = piece > 127;
        // break in the case the image is empty
        if (piece.empty()) {
            if (piece_index == 1) {
                cerr << "no file found with name: " << path << endl;
                exit(1);
            } else {
                cout << "total file read: " << piece_index << endl;
                break;
            }
        }

        Mat piece_with_no_knobs;
        remove_knobs(piece,piece_with_no_knobs);


        // find the 4 corners of the box using the minAreaRec function

        // canny filter to remove the pixels in the middle and make the execution faster
        Mat canny;
        Canny(piece_with_no_knobs,canny,50,200);
        //resize(canny,temp,Size(400,400));
        //imshow("kanny",temp);
        // finding the contortions of the various blobs;
        vector<vector<Point>> contours;
            findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
        vector<Point> main_contour = vector<Point>();
        // transforming it in a single vector
        for(auto v: contours){
            main_contour.insert(main_contour.end(),v.begin(),v.end());
        }
        // find the rectangle
        RotatedRect rect = minAreaRect( main_contour);
        // find the 4 points;
        Point2f vertices[4];
        rect.points(vertices);

        //  taking a mask of only the 4 angles;
        Mat angles = Mat::zeros(piece.size(),CV_8U);
        Mat angle_mask;
        Mat only_angle;

        int max_radius = 0;

        for(auto &point: vertices){
            int radius = 10;
            // finding the optimal radius to mask the image
            while (true){
                //making the mask empty
                angle_mask = Mat::zeros(piece.size(),CV_8U);
                //creating a circle around a point
                circle(angle_mask,point,radius,Scalar(255),-1);
                // mask containing only the angle
                bitwise_and(piece_with_no_knobs, angle_mask,only_angle);
                // if the angle is big enough i continue, otherwise i increase the radius
                if(countNonZero(only_angle) > 3000){
                    angles += only_angle;
                    break;
                }else{
                    radius*=13;
                    radius/=10;
                }
            }
            if(radius>max_radius){
                max_radius = radius;
            }
            //show(only_angle);
        }
        // just for safety increase it by 5
        max_radius += 5;

        // i need to increase the precision of the points... to do so i look for the corner by applying a blur to the corner
        // and detect the pixels that were white before the blur, but was black after, this means that they are in a sharp corner
        for(int i=0; i<4; i++) {
            // create a mask that keeps only a vertex at a time
            Mat single_corner;
            Mat mask = Mat::zeros(angles.size(), CV_8U);
            circle(mask, vertices[i], max_radius, Scalar(255), -1);
            bitwise_and(angles, mask, single_corner);

            //kernel for the blur
            kernel = Mat::zeros(Size(2*ANGLE_FINDING_BLUR_RADIUS,2*ANGLE_FINDING_BLUR_RADIUS),CV_32F);
            circle(kernel,Point(ANGLE_FINDING_BLUR_RADIUS,ANGLE_FINDING_BLUR_RADIUS),ANGLE_FINDING_BLUR_RADIUS,Scalar(1),-1);
            kernel /= countNonZero(kernel);

            // applying a blur;
            Mat blur;
            filter2D(piece_with_no_knobs, blur, CV_8U, kernel,Point(ANGLE_FINDING_BLUR_RADIUS,ANGLE_FINDING_BLUR_RADIUS),0,BORDER_ISOLATED);

            // masking the blur to consider only the piece close to an angle;
            Mat blur_masked;
            bitwise_and(blur,mask,blur_masked);

            //putting hhe bits outside of the original mask at 255
            temp = piece_with_no_knobs == 0;
            bitwise_or(blur_masked,temp,temp2);
            blur_masked = temp2;
            temp = mask == 0;
            bitwise_or(blur_masked,temp,temp2);
            blur_masked = temp2;
            // now i need to find the darkest pixel(s) coordinates, and that will be the new angle center
            Point minLoc;
            minMaxLoc(blur_masked, nullptr, nullptr, &minLoc, nullptr);
            vertices[i] = minLoc;

            //show(blur_masked);
        }


            //show(angles);

        // calculate the 4 vertices in a precis more precise way, by finding the smallest triangle that can contains the angle,
        // and tanking the closest point to the original point as new corner
        Point2f vertices_precise[4];
        for(int i=0; i<4; i++){
            // create a mask that keeps only a vertex at a time
            Mat single_corner;
            Mat mask = Mat::zeros(angles.size(),CV_8U);
            circle(mask,vertices[i],ANGLE_MASK_FOR_TRIANGLE_CALCULATION,Scalar(255),-1);

            bitwise_and(piece_with_no_knobs, mask, single_corner);

            resize(single_corner,temp,Size(400,400));
            //imshow(to_string(i),temp);

            // find the triangle with the minimum area that can keep inside the 3 dots
            // canny filter to remove the pixels in the middle and make the execution faster
            Canny(single_corner, canny, 50, 200);


            // finding the contortions of the points;
            findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
            // transforming it in a single vector
            main_contour = vector<Point>();
            for(auto v: contours){
                main_contour.insert(main_contour.end(),v.begin(),v.end());
            }

            // find the triangle that enclose the vertice
            vector<Point> triangle_points;
            minEnclosingTriangle( main_contour,triangle_points);
            //cout << triangle_points << endl;

            // Draw the triangle
            /*for( i = 0; i < 3; i++ )
                line(single_corner, triangle_points[i], triangle_points[(i + 1) % 3], Scalar(120), 1, LINE_AA);
            show(single_corner);*/

            // find the point that is the closest to the original point...
            // that point will be the vertex of the puzzle!
            Point closest = triangle_points[0];
            Point vertex = vertices[i];
            for(auto p: triangle_points){
                //find the closest point to the original vertex... that will be the precise vertex
                if(cv::norm(p - vertex) < cv::norm(closest - vertex)){
                    closest = p;
                }
            }
            vertices_precise[i] = closest;

        }


        cvtColor(piece,temp,COLOR_GRAY2BGR);

        for(int i=0; i<4; i++){
            line(temp,vertices_precise[i],ertices_precise[(i+1)%4],Scalar(0,0,255),8);
        }

        show(temp);

        // save the coordinates to a txt file
        path = string("../") + string(DIRECTORY) + string("/data/") + to_string(piece_index) + string(".txt");
        ofstream file;
        file.open (path,ios::out);
        file <<"P0: "<< vertices_precise[0] << endl;
        file <<"P1: "<< vertices_precise[1] << endl;
        file <<"P2: "<< vertices_precise[2] << endl;
        file <<"P3: "<< vertices_precise[3];
        file.close();

        piece_index++;
    }
}
