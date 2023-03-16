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

// takes many images in input and split the puzzle piece into single images
void split_pieces_into_single_images();

// take a single piece in inout and remove the holes
void remove_holes();

// take a single piece WITH the holes already removed, and remove the "extensions" remaining with a square
void remove_extensions_and_save_corner_data();

void show(Mat &m);

void quick_convex_hull(Mat &input, Mat &output_);

int main(){

    //split_pieces_into_single_images();

    //remove_holes();

    remove_extensions_and_save_corner_data();

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

        cout << number_of_pieces << endl;

        Mat temp = individual_pieces == number_of_pieces;


        // for each individual piece
        for(int i=1; i<number_of_pieces; i++){
            cout << "processing piece: " << piece_index << endl;

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

                //cout << "x1 x2 y1 y2: " << x1 << " " << x2  << " " << y1 << " " << y2 << endl;
                Mat cropped_image = single_piece(Range(y1,y2),Range(x1,x2));

                //show the piece corpped
                /*
                //Mat cropped_image_resized;
                Mat to_target_resized;
                //resize(cropped_image, cropped_image_resized, Size(400, 400));
                Mat to_target = raw_image;
                rectangle(to_target, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0, 0), 30);
                resize(to_target, to_target_resized, Size(550, 800));
                //imshow("output", cropped_image_resized);
                imshow("where", to_target_resized);
                waitKey(0);
                */

                //calculate path
                //string path = string("../") + string(DIRECTORY) + string("/divided/") + to_string(piece_index++) + string(IMAGE_FORMAT);

                //save the file
                imwrite(path,cropped_image);
            }
        }
    }
}


// the algorithm find the filler of a piece.
// a filler is an area that need to be included for the image to be considered convex
// the areas that will be calculated will be considered filler only if their area (in pixels) is greater than this threshold
#define FILLER_AREA_THRESHOLD 9000
// when a filler is found, it can be of 2 types...
// a filler created by a hole in the puzzle piece
#define EROSION_KERNEL_SIZE 100
#define MIN_SHRINKING_PERCENTAGE  0.44
//this function make some preprocessing to increase the speed of the comparison operation later
void remove_holes(){
    Mat temp;
    Mat temp2;
    Mat kernel;
    int piece_index = 1;

    while (true){

        cout << "processing piece: " << piece_index << endl;


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

        // find the convex hull of the original mask
        Mat convex_hull;
        quick_convex_hull(piece,convex_hull);

        // do a first erosion, to remove the thin area close to the puzzle hole
        Mat convex_hull_erosion;
        kernel = Mat::zeros(Size(EROSION_KERNEL_SIZE,EROSION_KERNEL_SIZE),CV_8U) == 0;
        erode(convex_hull,convex_hull_erosion,kernel);

        // the filler pieces that can make the original image a convex shape
        Mat filler;
        bitwise_xor(convex_hull,piece,filler);

        //erode and expand the border of the image to remove thin line
        Mat filler_eroded;
        kernel = Mat::zeros(Size(6,6),CV_8U) == 0;
        erode(filler,filler_eroded,kernel);
        Mat kernel2 = Mat::zeros(Size(6+1,6+1),CV_8U) == 0;
        dilate(filler_eroded,filler,kernel2);


        // split the filler into single piece
        Mat filler_split;
        int number_of_pieces = connectedComponents(filler,filler_split);
        // a copy of the original piece with removed holes
        Mat piece_no_hole = piece.clone();

        // decide if every piece is a hole or not, if is a hole i fill it up
        for (int i=1; i<number_of_pieces; ++i){
            Mat single_filler = filler_split == i;

            // ckeck that the filler is not just a thinly line due to a broder imperfection
            Mat remaining_filler;
            bitwise_and(convex_hull_erosion,single_filler,remaining_filler);

            // see comment on FILLER_AREA_THRESHOLD
            if(countNonZero(single_filler) > FILLER_AREA_THRESHOLD && countNonZero(remaining_filler) > 0){
                // calculating the distance from the most distance pixel in a filler and the border
                int last_zero_found = INT_MAX;
                int distance = 100;
                int distance_prev = 0;
                int last_non_zero_found = 0;
                while (distance != distance_prev){
                    distance_prev = distance;
                    Mat convex_hull_erosion_temp;
                    kernel = Mat::zeros(Size(distance,distance),CV_8U) == 0;
                    erode(convex_hull_erosion,convex_hull_erosion_temp,kernel);

                    // mask containing the pixels at a distance grater then `distance`
                    Mat masked_filler;
                    bitwise_and(convex_hull_erosion_temp,single_filler,masked_filler);

                    // display
                    /*resize(masked_filler,hole_removed,Size(500,500));
                    resize(convex_hull_erosion_temp,temp2,Size(500,500));
                    imshow("1",hole_removed);
                    imshow("2",temp2);
                    waitKey(0);*/


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
                erode(convex_hull_erosion,convex_hull_half_erosion,kernel);



                Mat single_filler_erosion_1, single_filler_erosion_2;
                bitwise_and(single_filler,convex_hull_erosion,single_filler_erosion_1);
                bitwise_and(single_filler,convex_hull_half_erosion,single_filler_erosion_2);
                // nota fai 2 shringk uno dopo l'altro, e fai il controllo tra le proporzioni solo tra il secondo e il terzo

                // see comment of MIN_CONVEX_PERCENTAGE for explanation
                float shrinking_percentage = (float)countNonZero(single_filler_erosion_2)/ (float)countNonZero(single_filler_erosion_1);

                //cout << shrinking_percentage << endl;

                /*resize(single_filler_erosion_1,hole_removed,Size(500,500));
                resize(single_filler_erosion_2,temp2,Size(500,500));
                imshow("1",hole_removed);
                imshow("2",temp2);
                waitKey(0);*/

                if(shrinking_percentage > MIN_SHRINKING_PERCENTAGE){
                    bitwise_or(piece_no_hole,single_filler,temp);
                    piece_no_hole = temp;
                }
            }
        }
        /*resize(piece_no_hole,hole_removed,Size(500,500));
        imshow("",hole_removed);
        waitKey(0);*/

        //calculate path
        string save_path = string("../") + string(DIRECTORY) + string("/hole_removed/") + to_string(piece_index++) + string(IMAGE_FORMAT);
        //save the file
        imwrite(save_path,piece_no_hole);

    }
}

// return if a mask is convex or not,
// it the function calculate the "convex percentage"
// witch is defined as the number of pixels of the original image decided by
// the number of pixels in the convex hull of the image
bool is_convex(Mat &input, float min_percentage = 0.9){
    Mat convex_hull;
    quick_convex_hull(input,convex_hull);
    return (float) countNonZero(input)/(float) countNonZero(convex_hull) > min_percentage;
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
    floodFill(output,Point(0,0),Scalar(100));
    output = output != 100;
}

#define EROSION_AND_EXPANSION_SIZE 75
#define RESIZE_DIVISION_FACTOR 6
#define SECOND_EROSION_SIZE 15
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
        kernel = Mat::zeros(Size(SECOND_EROSION_SIZE, SECOND_EROSION_SIZE), CV_8U);
        circle(kernel,Point(SECOND_EROSION_SIZE/2,SECOND_EROSION_SIZE/2),(SECOND_EROSION_SIZE-3)/2,Scalar(255),-1);
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
            // cout << "I: " << i  << " " << countNonZero(knob) << endl;
            if(countNonZero(knob) > MINIMUM_KNOB_AREA){
                // if it is an actual knob i remove an area from the original image

                //calculate center of the knob
                int c_y = stats.at<int>(i, cv::CC_STAT_TOP) + stats.at<int>(i, cv::CC_STAT_HEIGHT)/2;
                int c_x = stats.at<int>(i, cv::CC_STAT_LEFT) + stats.at<int>(i, cv::CC_STAT_WIDTH)/2;

                circle(piece_with_no_knobs,Point(c_x,c_y),KNOB_REMOVER_RADIUS,Scalar(0),-1);
            }
        }


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
            line(temp,vertices_precise[i],vertices_precise[(i+1)%4],Scalar(0,0,255),8);
        }

        show(temp);

        piece_index++;
    }
}

void show(Mat &m){
    Mat temp;
    resize(m,temp,Size(400,400));
    imshow("",temp);
    waitKey(0);
}