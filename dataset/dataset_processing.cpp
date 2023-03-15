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
void remove_extensions();

void show(Mat &m);

void quick_convex_hull(Mat &input, Mat &output_);

int main(){

    //split_pieces_into_single_images();

    //remove_holes();

    remove_extensions();

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

#define ANGLE_BLUR_SIZE 100

void remove_extensions(){
    Mat temp;
    Mat temp2;
    Mat kernel;
    int piece_index = 1;

    while (true) {

        cout << "processing piece: " << piece_index << endl;


        //step 1: read all the files
        string path =
                string("../") + string(DIRECTORY) + string("/hole_removed/") + to_string(piece_index) + string(IMAGE_FORMAT);
        // image with the scanned raw data
        Mat piece = imread(path, IMREAD_GRAYSCALE);
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

        /*
        kernel = Mat::zeros(Size(ANGLE_BLUR_SIZE, ANGLE_BLUR_SIZE), CV_32F);
        circle(kernel,Point(ANGLE_BLUR_SIZE/2,ANGLE_BLUR_SIZE/2),(ANGLE_BLUR_SIZE-1)/2,Scalar(1),1);
        kernel /= countNonZero(kernel);

        Mat piece_blur, piece_32F;
        piece.convertTo(piece_32F,CV_32F);
        Size big_size = piece_32F.size() + Size(ANGLE_BLUR_SIZE*4,ANGLE_BLUR_SIZE*4);
        Mat piece_expanded = Mat::zeros(big_size,CV_32F);
        Rect rect = Rect (ANGLE_BLUR_SIZE*2,ANGLE_BLUR_SIZE*2, piece_32F.size().width,piece_32F.size().height);
        Mat middle_point_of_piece_expanded = Mat(piece_expanded,rect);
        piece_32F.copyTo(middle_point_of_piece_expanded);
        //cout << piece_32F;
        filter2D(piece_expanded,piece_blur,CV_32F ,kernel,Point(ANGLE_BLUR_SIZE/2,ANGLE_BLUR_SIZE/2));
        piece_blur.convertTo(temp,CV_8U);
        resize(temp,temp2,Size(400,400));
        imshow(" ", temp2);

        piece_blur = piece_blur(Range(ANGLE_BLUR_SIZE*2,piece_32F.size().height+ANGLE_BLUR_SIZE*2),Range(ANGLE_BLUR_SIZE*2,piece_32F.size().width+ANGLE_BLUR_SIZE*2));

        //cout << piece_blur;


        //Mat piece_threshold = piece_blur > 255.0*0.375;
        Mat piece_threshold = piece_blur > 255.0*0.55;


        Mat offset;
        bitwise_and(255-piece,piece_threshold,offset);

        show(offset);
        */




        /*
        int a = 400;
        kernel = Mat::zeros(Size(a,a),CV_8U);
        circle(kernel,Point(a/2,a/2),a/2,Scalar(255),-1);
        Mat dilated, eroded;
        erode(piece,eroded,kernel);
        dilate(eroded,dilated,kernel);

        resize(dilated,temp2,Size(400,400));
        imshow(" ",temp2);
        show(piece);
        //waitKey(0);
         */

        // metodo identifica centro protuberanza
        /*
        int a = 200;
        kernel = Mat::zeros(Size(a,a),CV_8U);
        circle(kernel,Point(a/2,a/2),a/2,Scalar(255),-1);
        Mat corner, eroded;
        erode(piece,eroded,kernel);
        corner = piece - eroded;
        //show(corner);

        a = 200;
        kernel = Mat::zeros(Size(a,a),CV_32F);
        circle(kernel,Point(a/2,a/2),75,Scalar(1),20);
        kernel /= countNonZero(kernel);

        temp = kernel > 0;
        show(temp);


        corner = 255 - corner;
        corner.convertTo(temp,CV_32F);
        corner = temp;

        Mat output;
        filter2D(corner,output,CV_32F,kernel,Point(a/2,a/2));

        output.convertTo(temp,CV_8U);
        output = temp;

        resize(piece,temp,Size(400,400));
        imshow("  ",temp);
        show(output);
        */




        resize(piece,temp,Size(200,200));
        floodFill(temp,Point(0,0),Scalar(100));
        piece = temp != 100;



        int k = 75;
        kernel = Mat::zeros(Size(k, k), CV_8U);
        circle(kernel,Point(k/2,k/2),(k-3)/2,Scalar(255),-1);


        Mat piece_blur, piece_32F;
        piece.convertTo(piece_32F,CV_32F);
        Size big_size = piece_32F.size() + Size(k*4,k*4);
        Mat piece_expanded = Mat::zeros(big_size,CV_32F);
        Rect rect = Rect (k*2,k*2, piece_32F.size().width,piece_32F.size().height);
        Mat middle_point_of_piece_expanded = Mat(piece_expanded,rect);
        piece_32F.copyTo(middle_point_of_piece_expanded);
        //cout << piece_32F;ù
        //imshow("!",piece_expanded);

        erode(piece_expanded,temp,kernel);
        dilate(temp,piece_blur,kernel);

        piece_blur.convertTo(temp,CV_8U);
        resize(temp,temp2,Size(400,400));
        imshow(" ", temp2);

        piece_blur = piece_blur(Range(k*2,piece_32F.size().height+k*2),Range(k*2,piece_32F.size().width+k*2));

        //cout << piece_blur;


        //Mat piece_threshold = piece_blur > 255.0*0.375;
        Mat piece_threshold = piece_blur > 255.0*0.55;

        temp = piece_threshold == 0;
        bitwise_and(piece,temp,temp2);

        k = 15;
        kernel = Mat::zeros(Size(k, k), CV_8U);
        circle(kernel,Point(k/2,k/2),(k-3)/2,Scalar(255),-1);

        erode(temp2,temp,kernel);

        imshow(" ",temp);
        show(piece_threshold);





        piece_index++;
    }
}

void show(Mat &m){
    Mat temp;
    resize(m,temp,Size(400,400));
    imshow("",temp);
    waitKey(0);
}