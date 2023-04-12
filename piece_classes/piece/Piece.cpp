
#include "Side.h"
#include "Piece.h"
#include <fstream>
#include <utility>

using namespace std;
using namespace cv;

string Piece::origin_path = "";

/// how much the original image will be resized for the preview
#define PREVIEW_RESIZE_FACTOR 4
#define ROTATION_RESOLUTION 2000

Piece::Piece(int piece_id, string path) {
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

    //calculate offset of the piece
    Point center = (points[0]+points[1]+points[2]+points[3])/4;

    // create sides
    for(int i=0; i<4; i++){
        sides[i] = Side(piece, this,i,points[i],points[(i+1)%4],center);
    }


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

    // doing resize for faster operations later
    Mat temp;
    resize(piece,temp,piece.size()/PREVIEW_RESIZE_FACTOR);
    piece=temp;

    for(auto &p:points){
        p/=PREVIEW_RESIZE_FACTOR;
    }

}

Piece::Piece(int piece_id): Piece(piece_id, origin_path){}

void Piece::set_origin_path(string path) {
    origin_path = std::move(path);
}

Side &Piece::get_side(int index) {
    assert(index >= 0);
    assert(index <4);
    return sides[index];
}

void Piece::show_debug(int side_to_highlight) {
    Mat to_show;
    cvtColor(piece,to_show,COLOR_GRAY2BGR);
    for(int i=0; i<4; i++){
        Scalar color;
        if(side_to_highlight == i){
            color = Scalar(0,255,0);
        }else{
            color = Scalar(0,0,255);
        }
        line(to_show,points[i],points[(i+1)%4],color,4);
    }
    Mat resized;
    resize(to_show,resized,to_show.size()/2);
    imshow("Debug", resized);
    waitKey(0);
}

cv::Mat Piece::get_image() {
    return this->piece;
}

cv::Point Piece::get_point(int index) {
    assert(index>=0);
    assert(index<4);
    return points[index];
}

int Piece::get_preview_x_res() {
    return ROTATION_RESOLUTION/PREVIEW_RESIZE_FACTOR;
}

int Piece::get_preview_y_res() {
    return ROTATION_RESOLUTION/PREVIEW_RESIZE_FACTOR;
}
