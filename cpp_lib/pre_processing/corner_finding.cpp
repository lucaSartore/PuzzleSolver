//
// Created by luca on 3/17/23.
//
#include "corner_finding.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "util.h"
#include "piece_splitting.h"
#include "corner_finding.h"
#include "../solving/puzzle_preview/PreviewManager.h"
#include <thread>
#include "../solving/graphic_piece/PieceShape.h"
#include <fstream>
#include <utility>

// if this is defined the program will insert debug images
//#define DEBUG

using namespace std;
using namespace cv;

void do_pre_processing_thread(const std::string& path, int piece_index, int ppi = DEFAULT_PPI, bool enable_image_view = false);
void remove_holes(const cv::Mat &input, cv::Mat &output,int ppi = DEFAULT_PPI, bool enable_image_view = false);
void remove_knobs(const cv::Mat &input, cv::Mat &output, int ppi = DEFAULT_PPI, bool enable_image_view = false);
void find_corners(const cv::Mat &input, cv::Point &p1,  cv::Point &p2,  cv::Point &p3,  cv::Point &p4, int ppi = DEFAULT_PPI, bool enable_image_view = false);

// handle the resize and the condition for a quick preview
void quick_image_preview(const cv::Mat &image, bool enable_preview, const string& name_of_image_to_save = ""){
    if(!enable_preview){
        return;
    }


    if(!name_of_image_to_save.empty()){
        imwrite(name_of_image_to_save + ".png", image);
    }

    Mat resize;
    cv::resize(image,resize,image.size()/3);

    imshow("preview", resize);
    waitKey(0);
}

void find_corners(const std::string& path, int number_of_pieces, int ppi, int number_of_cores, bool enable_image_view){

    // impossible to enable the view if multi threading is in use!
    if(number_of_cores != 1){
        assert(enable_image_view == false);
    }

    if(number_of_cores != 1){

        // if user select 0: automatic detecting
        if (number_of_cores == 0){
            number_of_cores = (int) std::thread::hardware_concurrency();
        }

        // make share the number is detected correctly
        assert(number_of_cores != 0);

        int piece_number = 0;

        while (piece_number < number_of_pieces){

            // find how many threads i have to spawn
            int threads_to_spawn = number_of_cores;

            // create the threads
            auto *threads = new thread[threads_to_spawn];

            for(int j=0; j<threads_to_spawn; j++){

                // make share not to overflow
                if(piece_number >= number_of_pieces){
                    break;
                }

                auto f = [path,piece_number](){ do_pre_processing_thread(path,piece_number);};
                threads[j] = thread(f);
                cout << "preprocessing piece " << piece_number << "/" << number_of_pieces << endl;

                piece_number++;
            }
            for(int j=0; j<threads_to_spawn; j++){
                if(threads[j].joinable()){
                    threads[j].join();
                }
            }

            // free the memory
            delete[] threads;
        }

    }else{
        for(int i=0; i<number_of_pieces; i++){
            cout << "preprocessing piece " << i << "/" << number_of_pieces << endl;

            do_pre_processing_thread(path, i, ppi, enable_image_view);
        }
    }
}

void do_pre_processing_thread(const std::string& path, int piece_index, int ppi, bool enable_image_view){

    // load the piece
    string piece_path = path + string("/") +  to_string(piece_index) + string(".jpeg");
    Mat piece = imread(piece_path, IMREAD_GRAYSCALE);
    assert(! piece.empty());

    // removing the hole of the piece
    Mat piece_no_holes;
    remove_holes(piece,piece_no_holes,ppi,enable_image_view);

    // removing the knobs
    Mat piece_no_knobs;
    remove_knobs(piece_no_holes,piece_no_knobs,ppi,enable_image_view);

    // finding the points
    Point p1,p2,p3,p4;
    find_corners(piece_no_knobs,p1,p2,p3,p4,ppi,enable_image_view);

    // show results
    if(enable_image_view){
        Mat display;
        cvtColor(piece,display,COLOR_GRAY2BGR);

        Scalar color = Scalar(0,0,255);
        int thickness = 10;

        line(display,p1,p2,color,thickness);
        line(display,p2,p3,color,thickness);
        line(display,p3,p4,color,thickness);
        line(display,p4,p1,color,thickness);

        Mat display_resize;
        resize(display,display_resize,display.size()/(3*ppi/1200));
        imshow("piece with corners", display_resize);
        waitKey(0);
    }
    if(PreviewManager::is_preview_enabled()){
        Mat display;
        cvtColor(piece,display,COLOR_GRAY2BGR);

        Scalar color = Scalar(0,0,255);
        int thickness = 10;

        line(display,p1,p2,color,thickness);
        line(display,p2,p3,color,thickness);
        line(display,p3,p4,color,thickness);
        line(display,p4,p1,color,thickness);

        Mat display_resize;
        resize(display,display_resize,display.size()/(2*ppi/1200));
        PreviewManager::output_preview_image(display_resize);
    }

    // save the coordinates to a txt file
    string save_path = path + string("/") + to_string(piece_index) + string(".txt");

    ofstream file;
    file.open (save_path,ios::out);
    file <<"P1: "<< p1 << endl;
    file <<"P2: "<< p2 << endl;
    file <<"P3: "<< p3 << endl;
    file <<"P4: "<< p4;
    file.close();
}


void convert_coordinates_to_json(const std::string& input_path, int number_of_pieces, const std::string& output_path){
    PieceShape::set_origin_path(input_path);

    ofstream file;
    file.open (output_path,ios::out);

    file<<"["<<endl;

    for(int i=0; i<number_of_pieces; i++){


        string data_path = input_path + string("\\") + to_string(i) + string(".txt");

        // get coordinates
        string line;
        ifstream data = ifstream(data_path);

        assert(data.is_open());

        Point points[4];

        // read the points
        for (auto & point : points) {
            std::getline(data, line);
            std::stringstream ss(line);
            char c[6], a;
            int x, y;
            ss >> c >> a >> x >> a >> y >> a;
            point = Point(x, y);
        }

        file << "\t{" << std::endl;
        file << "\t\t\"piece_id\": " << i << "," << std::endl;
        for(int j = 0; j < 4; j++){

            auto p1 = points[j];
            auto p2 =points[(j + 1) % 4];

            file << "\t\t\"side_" << j << "\": {" << std::endl;
            file << "\t\t\t\"p1\": {\"x\":" << p1.x << ", \"y\":" << p1.y << "}," << std::endl;
            file << "\t\t\t\"p2\": {\"x\":" << p2.x << ", \"y\":" << p2.y << "}" << std::endl;

            if(j < 3)
                file << "\t\t}," << std::endl;
            else
                file << "\t\t}" << std::endl;
        }

        if(i < number_of_pieces - 1)
            file << "\t}," << std::endl;
        else
            file << "\t}" << std::endl;

    }

    file<<"]" << endl;
}

// this function remove the holes of the puzzle_preview in the following way:
// - the algorithm find the filler of a piece.
//         a filler is an area that need to be included for the image to be considered convex
// - i erode them by FIRST_EROSION_KERNEL_SIZE and expand them by FIRST_EROSION_KERNEL_SIZE + 1 to remove tiny lines
// - then i split all the mask into all the separated pieces, and consider them only if their area in pixels is grater
//   then `FILLER_AREA_THRESHOLD`
// - i erode the area with a kernel of `SECOND_EROSION_KERNEL_SIZE` so that the small lines (like the one close to a hole in
//   the puzzle_preview get removed
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
///this function removes the "holes" of the puzzle_preview, so that it could be processed easily later
void remove_holes(const Mat &input, Mat &output,const int ppi, bool preview_enable){
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

    quick_image_preview(original_convex_hull,preview_enable,"remove_holes_convex_hull");

    // the filler pieces that can make the original image a convex shape
    Mat filler;
    bitwise_xor(original_convex_hull,piece,filler);

    quick_image_preview(filler,preview_enable,"remove_holes_filler_areas");


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

        // do a first erosion, to remove the thin area close to the puzzle_preview hole
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

    quick_image_preview(piece_no_hole,preview_enable,"piece_with_no_hole");

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
void remove_knobs(const cv::Mat&input, cv::Mat &output,const int ppi, bool enable_image_view){

    Mat piece = input, kernel,temp;

    #ifdef DEBUG
    show(piece);
    #endif

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

    #ifdef DEBUG
        show(temp);
    #endif

    quick_image_preview(temp,enable_image_view,"remove_knobs_erosion");
    dilate(temp,piece_with_smooth_corner,kernel);
    #ifdef DEBUG
        show(piece_with_smooth_corner);
    #endif
    quick_image_preview(piece_with_smooth_corner,enable_image_view,"remove_knobs_expansion");

    // bumps_along_corner = piece_resize AND ( NOT piece_with_smooth_corner)
    // this mask now contains the bumps, and the angles of the original image
    Mat bumps_along_corner;
    temp = piece_with_smooth_corner == 0;
    bitwise_and(piece_resize,temp,bumps_along_corner);

    #ifdef DEBUG
        show(bumps_along_corner);
    #endif

    quick_image_preview(bumps_along_corner,enable_image_view,"remove_knobs_knob_pixels");


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
    #ifdef DEBUG
        show(piece_with_no_knobs);
    #endif
    quick_image_preview(piece_with_no_knobs,enable_image_view,"piece_with_no_knobs");

    output = piece_with_no_knobs;
};



// this function find the 4 corners in the image, it dose using 3 layer of precision... this means that it find the
// approximate position of the corners once, than it uses that data to find a mor precise position, and it do so a third
// time, so he can end up with a precise estimate
//
// the 3 layer of precision are the following:
//
// 1) it find the rectangle that can contain all of the maks, with the minimum possible area, and it then i take the 4
//    corner of the rectangle as first approximation
// 2) i den apply a blur with a radius of `ANGLE_FINDING_BLUR_RADIUS` and find the coordinates of the darkest pixel
//    that was white before, and is among the `MIN_NUMBER_OF_PIXELS_TO_CONSIDER_AS_CORNER` closest pixels to the previews
//    position of the corner. this pixel will be the new corner
// 3) i create a mask with a radius of `ANGLE_MASK_FOR_TRIANGLE_CALCULATION` around the corner coordinates, use it to
//    mask the original input, and then find tha smallest triangle that can contain the masked area. i then take the
//    corner of the triangle that is the closest to the previews position, and that is the final point
#define ANGLE_FINDING_BLUR_RADIUS 150
#define ANGLE_MASK_FOR_TRIANGLE_CALCULATION 100
#define MIN_NUMBER_OF_PIXELS_TO_CONSIDER_AS_CORNER 3000
/// tis function return the precise coordinates of the 4 angle of the puzzle_preview piece
/// the image in input must have the knob and hole removed
void find_corners(const cv::Mat &input, cv::Point &p1,  cv::Point &p2,  cv::Point &p3,  cv::Point &p4,const int ppi, bool enable_image_view){

    Mat piece_with_no_knobs = input, kernel,temp,temp2;


    //
    //      APPROXIMATION 1
    //

    // canny filter to remove the pixels in the middle and make the execution faster
    Mat canny;
    Canny(input,canny,50,200);

    // find the contours and put it in to a 1D array;
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

    if(enable_image_view){
        Mat preview;
        cvtColor(piece_with_no_knobs,preview,COLOR_GRAY2BGR);

        for(int i=0; i<4; i++){
            auto p1 = vertices[i];
            auto p2 = vertices[(i+1)%4];

            line(preview,p1,p2,Scalar(0,0,255),15);
        }
        quick_image_preview(preview,enable_image_view,"find_corners_min_enclosing_rectangle");
    }


    //
    //      APPROXIMATION 2
    //

    //  taking a mask of only the 4 angles;

    // mask with all 4 angles
    Mat angles = Mat::zeros(input.size(),CV_8U);
    // mask for masking one angle at a time
    Mat angle_mask;
    // mask with an angle at a time
    Mat single_angle;

    // need to keep track of the biggest radius used to identify the `MIN_NUMBER_OF_PIXELS_TO_CONSIDER_AS_CORNER` closest pixels to the corner
    int max_radius = 0;

    for(auto &point: vertices){
        int radius = 10;
        // finding the optimal radius to mask the image
        while (true){

            //making the mask empty
            angle_mask = Mat::zeros(input.size(),CV_8U);

            //creating a circle around a point
            circle(angle_mask,point,radius,Scalar(255),-1);

            // mask containing only the angle
            bitwise_and(piece_with_no_knobs, angle_mask, single_angle);

            // if the angle is big enough i continue, otherwise i increase the radius
            if(countNonZero(single_angle) > MIN_NUMBER_OF_PIXELS_TO_CONSIDER_AS_CORNER){
                angles += single_angle;
                break;
            }else{

                radius*=13;
                radius/=10;
            }
        }
        if(radius>max_radius){
            max_radius = radius;
        }
    }
    // just for safety increase it by 5.. the angle will be so distance that there will be no differnece
    max_radius += 5;

    // i need to increase the precision of the points... to do so i look for the corner by applying a blur to the corner
    // and detect the pixels that were white before the blur, but was black after, this means that they are in a sharp corner
    for(auto & vertex : vertices) {
        // create a mask that keeps only a vertex at a time
        Mat single_corner;
        Mat mask = Mat::zeros(angles.size(), CV_8U);
        circle(mask, vertex, max_radius, Scalar(255), -1);
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
        vertex = minLoc;

        //show(blur_masked);
    }

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

        if(enable_image_view){
            Mat preview;
            cvtColor(single_corner,preview,COLOR_GRAY2BGR);

            for(int i=0; i<3; i++){
                auto p1 = triangle_points[i];
                auto p2 = triangle_points[(i+1)%3];

                line(preview,p1,p2,Scalar(0,0,255),4);
            }
            quick_image_preview(preview,enable_image_view,"find_corners_min_enclosing_triangle");
        }


        // find the point that is the closest to the original point...
        // that point will be the vertex of the puzzle_preview!
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


    p1 = vertices_precise[0];
    p2 = vertices_precise[1];
    p3 = vertices_precise[2];
    p4 = vertices_precise[3];
}


void export_sides_result(const std::string& input_images_path,const std::string& input_coordinates_path,const std::string& output_path, int number_of_pieces){

    for(int i=0; i<number_of_pieces;i++){

        //############## get coordinates ###################
        Point points[4];
        string line;
        ifstream data (input_coordinates_path + "/" + to_string(i) + ".txt");

        assert(data.is_open());

        // read the points
        for (auto & point : points) {
            std::getline(data, line);
            std::stringstream ss(line);
            char c[6], a;
            int x, y;
            ss >> c >> a >> x >> a >> y >> a;
            point = Point(x, y);
        }

        //calculate center of the piece
        Point center = (points[0]+points[1]+points[2]+points[3])/4;

        //############## get image ###################

        Mat image = imread(input_images_path + "/" + to_string(i) + ".jpeg");


        for(int side=0; side<4; side++){
            Point p1 = points[side];
            Point p2 = points[(side+1)%4];

            Mat border_shape = Mat::zeros(1500, 1500, image.type());

            // Calculate the midpoint between p1 and p2
            float midX = (p1.x + p2.x) / 2.0f;
            float midY = (p1.y + p2.y) / 2.0f;

            // Calculate the angle between p1 and p2
            float angle = std::atan2(p2.y - p1.y, p2.x - p1.x) * 180.0f / CV_PI;

            // Create a transformation matrix to rotate and move the input image
            // docs: https://docs.opencv.org/3.4/da/d54/group__imgproc__transform.html#gafbbc470ce83812914a70abfb604f4326
            cv::Mat transform = cv::getRotationMatrix2D(cv::Point2f((p1+p2)/2), angle, 1.0);
            transform.at<double>(0, 2) += (float)1500/2 - midX;
            transform.at<double>(1, 2) += (float)1500/2 - midY;

            // Apply the transformation to the input image and put it in the output image
            warpAffine(image, border_shape, transform, border_shape.size());

            imwrite(output_path + "/" + to_string(i) + "_" + to_string(side) + ".jpeg", border_shape);
        }

    }
}