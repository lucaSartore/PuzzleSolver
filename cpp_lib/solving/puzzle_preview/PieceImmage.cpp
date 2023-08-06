//
// Created by luca on 4/25/23.
//

#include "PieceImage.h"
#include <fstream>
#include "../constants.h"


using namespace std;
using namespace cv;

string PieceImage::origin_path = "";

PieceImage::PieceImage(int piece_id) : PieceImage(piece_id,origin_path){
}

PieceImage::PieceImage(int piece_id, std::string path) {
    string piece_path = path + string("/") + to_string(piece_id) + string(".jpeg");
    string data_path = path + string("/") + to_string(piece_id) + string(".txt");

    // initialize piece
    piece = imread(piece_path,IMREAD_GRAYSCALE);
    assert(!piece.empty());

    // initialize id
    id = piece_id;

    // get coordinates
    string line;
    ifstream data (data_path);

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


    Mat new_piece = Mat::zeros(ROTATION_RESOLUTION,ROTATION_RESOLUTION,CV_8U);

    // Calculate the correction angle, by averaging the angle between the 2 diagonals
    float angle_error_on_diagonal_1 = std::atan2(points[1].y - points[3].y, points[1].x - points[3].x)-CV_PI/4.0;
    float angle_error_on_diagonal_2 = std::atan2(points[2].y - points[0].y, points[2].x - points[0].x)+CV_PI/4;

    float angle = (angle_error_on_diagonal_2+angle_error_on_diagonal_1)/2*180/CV_PI;

    // Create a transformation matrix to rotate and move the input image
    // docs: https://docs.opencv.org/3.4/da/d54/group__imgproc__transform.html#gafbbc470ce83812914a70abfb604f4326
    cv::Mat transform = cv::getRotationMatrix2D(center, angle, 1.0);
    transform.at<double>(0, 2) += ROTATION_RESOLUTION/2 - center.x;
    transform.at<double>(1, 2) += ROTATION_RESOLUTION/2 - center.y;

    // Apply the transformation to the input image and put it in the output image
    warpAffine(piece, new_piece, transform, new_piece.size());

    //put the piece back on place
    piece = new_piece;


    // transform the coordinates as well

    // translating my coordinates to the new form
    std::vector<Point2f> points_vector = std::vector<Point2f>(),  points_vector_transformed = std::vector<Point2f>();
    for(auto p: points){
        points_vector.push_back((Point2f)p);
    }
    cv::transform(points_vector,points_vector_transformed,transform);
    for(int i=0; i<4; i++){
        points[i] = points_vector_transformed[i];
    }

    Point image_center = (points[0]+points[1]+points[2]+points[3])/4;


    if (PUT_DEBUG_NUMBER_INSIDE_THE_PIECE){
        cerr << "warning: PUT_DEBUG_NUMBER_INSIDE_THE_PIECE is enable... this would meake the comparason between pieces inaccurate" << endl;
        // DEBUG: put number of piece on top:
        putText(piece, to_string(piece_id), image_center-Point(20,-20), FONT_HERSHEY_COMPLEX,4, cv::Scalar(0, 0, 0),13);
        // line to avoid floadfeel daling
        cv::line(piece,image_center-Point(0,2),image_center+Point(0,50),Scalar(255,255,255),3);
        for(int i=0; i<4; i++){
            Point side_point = (points[i]+points[(i+1)%4]+image_center*4)/6-Point(20,-20);
            putText(piece, to_string(i), side_point, FONT_HERSHEY_COMPLEX,3, cv::Scalar(0, 0, 0),7);
        }
    }


    // doing resize for faster operations later
    Mat temp;
    resize(piece,piece_resized,piece.size()/PREVIEW_RESIZE_FACTOR);

    for(int i=0; i<4; i++){
        points_resized[i] = points[i] / PREVIEW_RESIZE_FACTOR;
    }
}

void PieceImage::set_origin_path(std::string path) {
    origin_path = std::move(path);
}


cv::Mat PieceImage::get_image() {
    return piece;
}

cv::Mat PieceImage::get_image_resized() {
    return piece_resized;
}



cv::Point PieceImage::get_point(int index) {
    assert(index>=0);
    assert(index<4);
    return points[index];
}

cv::Point PieceImage::get_point_resized(int index) {
    assert(index>=0);
    assert(index<4);
    return points_resized[index];
}


int PieceImage::get_preview_x_res() {
    return ROTATION_RESOLUTION/PREVIEW_RESIZE_FACTOR;
}

int PieceImage::get_preview_y_res() {
    return ROTATION_RESOLUTION/PREVIEW_RESIZE_FACTOR;
}

int PieceImage::get_full_x_res() {
    return ROTATION_RESOLUTION;
}

int PieceImage::get_full_y_res() {
    return ROTATION_RESOLUTION;
}

int PieceImage::get_id() {
    return id;
}

