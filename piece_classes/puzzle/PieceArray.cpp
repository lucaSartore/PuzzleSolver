#include "PieceArray.h"
#include "stdexcept"
#include "UnknownHolder.h"
#include "OutsideHolder.h"
#include "PieceHolder.h"
#include <memory>
using namespace std;
using namespace cv;


/// how many pixels the image will grow when it runs out of real estate for the new pieces
#define GROWTH_CONSTANT 2000
/// the original dimension of the image
#define STARTING_DIMENSIONS 5000
/// the distance between the pieces and the borders
#define BORDER_DISTANCE 250
/// the number of pixels the fo margin there has to be before the image resolution is increase
#define MARGIN_BEFORE_GROWTH 2000

PieceArray::PieceArray() {

    // create the outside holder
    outside_tile = new OutsideHolder();

    dim_x = 1;
    dim_y = 1;
    pieces = vector<vector<shared_ptr<Holder>>>();

    // start with a 1*1 array with a null ptr inside
    auto one_dim = vector<shared_ptr<Holder>>();
    one_dim.push_back(shared_ptr<Holder>(new UnknownHolder()));
    pieces.push_back(one_dim);

    image = Mat::zeros(Size(STARTING_DIMENSIONS,STARTING_DIMENSIONS),CV_8UC3);

    srand(time(NULL));
}

Holder *PieceArray::get(int x, int y) const{



    // check if in range
    try{
        check_indexes(x,y);
    }catch (...){
        // if they are outside, but by one border at most, i return a border, otherwise is an error
        try{
            check_indexes(x-1,y);
            return outside_tile;
        }catch(...){}
        try{
            check_indexes(x+1,y);
            return outside_tile;
        }catch(...){}
        try{
            check_indexes(x,y-1);
            return outside_tile;
        }catch(...){}
        try{
            check_indexes(x,y+1);
            return outside_tile;
        }catch(...){}
        // if none of them succeed, i throw the normal error
        check_indexes(x,y);
    }


    // get the value
    shared_ptr<Holder> to_return = pieces[x][y];
    if(to_return == nullptr){
        cerr << "index at X: " << x << " y: " << y << " is null ptr" << endl;
        throw std::runtime_error("specified index is null ptr");
    }
    return &(*to_return);
}

void PieceArray::set(int x, int y, std::shared_ptr<Holder> to_be_set) {
    // check if in range
    check_indexes(x,y);
    pieces[x][y] = to_be_set;
}

void PieceArray::remove(int x, int y) {
    // check if in range
    check_indexes(x,y);

    // insert a new unknown value
    pieces[x][y] = shared_ptr<Holder>(new UnknownHolder());
}

void PieceArray::check_indexes(int x, int y) const {
    // check if indexes are in range
    if(x<0 || y<0 || x >= dim_x || y >= dim_y){
        cerr << "index at X: " << x << " y: " << y << " is out of range for array with size: X: " << dim_x << " Y: " << dim_y << endl;
        throw std::invalid_argument("index out of range");
    }
}

void PieceArray::grow_x() {
    auto new_colon = vector<shared_ptr<Holder>>();
    for(int i=0; i<dim_y; i++){
        new_colon.push_back(shared_ptr<Holder>(new UnknownHolder()));
    }
    pieces.push_back(new_colon);
    dim_x++;
}

void PieceArray::grow_y() {
    for(int i=0; i<dim_x; i++){
        pieces[i].push_back(shared_ptr<Holder>(new UnknownHolder()));
    }
    dim_y++;
}

int PieceArray::get_dim_x() const{
    return dim_x;
}

int PieceArray::get_dim_y() const  {
    return dim_y;
}

void PieceArray::un_grow_x() {
    if(dim_x != 0){
        pieces.pop_back();
        dim_x--;
    }
}

void PieceArray::un_grow_y() {
    if(dim_y != 0){
        for(int i=0; i<dim_x; i++){
            pieces[i].pop_back();
        }
        dim_y--;
    }
}

PieceArray::~PieceArray() {
    delete outside_tile;
}

cv::Mat PieceArray::get_image() const {
    return image.clone();
}

void PieceArray::build_image() {

}

void PieceArray::paste_on_top(const cv::Mat& source, cv::Mat& destination, cv::Point2i pointSource, cv::Point2i pointDestination,bool bitwise_or) {

    // Compute the translation vector to align the two points
    cv::Point2i translation = pointDestination - pointSource;

    cv::Mat croppedDestination = destination(
            cv::Rect(
                    translation.x,
                    translation.y,
                    source.size().width,
                    source.size().height
                     )
            );

    if(bitwise_or){
        // use bitwise or to avoid contrasts
        Mat result;
        cv::bitwise_or(source,croppedDestination,result);
        result.copyTo(croppedDestination);
    }else{
        // Paste the cropped source image onto the cropped destination image
        source.copyTo(croppedDestination);

    }

}

void PieceArray::check_if_grow() {
    Mat image_gray;
    cvtColor(image,image_gray,COLOR_BGR2GRAY);
    image_gray = image_gray!=0;
    Rect bounding_rectangle = boundingRect(image_gray);
    int x_0 = bounding_rectangle.x;
    int y_0 = bounding_rectangle.y;
    int x_1 = x_0 + bounding_rectangle.width;
    int y_1 = y_0 + bounding_rectangle.height;


    Size im_size = image.size();

    // if need to grow the x res
    if(x_1>im_size.width-MARGIN_BEFORE_GROWTH){
        // creating new bigger image
        Mat new_image = Mat::zeros(Size(im_size.width + GROWTH_CONSTANT,im_size.height), CV_8UC3);
        // pasting the old piece on top of the old piece
        paste_on_top(new_image,image,Point2i(0,im_size.height-1),Point2i(0, new_image.size().height-1));
        // changing the original image
        image = new_image;
    }

    // updating image size
    im_size = image.size();

    // if need to grow the x res
    if(y_1>im_size.width-MARGIN_BEFORE_GROWTH){
        // creating new bigger image
        Mat new_image = Mat::zeros(Size(im_size.width,im_size.height + GROWTH_CONSTANT), CV_8UC3);
        // pasting the old piece on top of the old piece
        paste_on_top(new_image,image,Point2i(0,0),Point2i(0, 0));
        // changing the original image
        image = new_image;
    }
}

void PieceArray::insert_into_image(int x, int y) {
    check_indexes(x,y);

    // get the piece on the top and bottom of the piece i'm trying to place
    Holder* piece_left = get(x-1,y);
    Holder* piece_top = get(x, y - 1);

    Holder* this_piece = get(x,y);

    if(piece_top->is_unknown() || piece_left->is_unknown()){
        throw invalid_argument("one of the pieces involved is unknown!");
    }
    if(!this_piece->is_a_piece()){
        throw invalid_argument("there is not a piece at the specified coordinates");
    }

    auto* this_piece_cast = dynamic_cast<PieceHolder*>(this_piece);

    // vector that go form the right side of the current piece to the center
    Point left_to_center_vector = this_piece_cast->get_center()-this_piece_cast->get_side_center(LEFT);
    // vector that go form the top side of the current piece to the center
    Point top_to_center_vector = this_piece_cast->get_center()-this_piece_cast->get_side_center(UP);

    int center_x,center_y;


    if(piece_left->is_outside() && piece_top->is_outside()){
        // default case: i need to place a corner

        // calculating the point of where to put the new piece
        center_x = BORDER_DISTANCE + left_to_center_vector.x;
        center_y = BORDER_DISTANCE + top_to_center_vector.y;

    }else if(piece_left->is_outside()){
        // default case: i need place a border vertically

        auto* piece_top_upcast = dynamic_cast<PieceHolder*>(piece_top);

        center_x = BORDER_DISTANCE + left_to_center_vector.x;
        center_y = (piece_top_upcast->get_side_center_with_offset(DOWN) + top_to_center_vector).y;
    }else if(piece_top->is_outside()){
        // default case: i need to place a border horizontally

        auto* piece_left_upcast = dynamic_cast<PieceHolder*>(piece_left);

        center_x = (piece_left_upcast->get_side_center_with_offset(RIGHT) + left_to_center_vector).x;
        center_y = BORDER_DISTANCE + top_to_center_vector.y;
    }else{
        auto* piece_left_upcast = dynamic_cast<PieceHolder*>(piece_left);
        auto* piece_top_upcast = dynamic_cast<PieceHolder*>(piece_top);

        Point center_1 = piece_left_upcast->get_side_center_with_offset(RIGHT) + left_to_center_vector;
        Point center_2 = piece_top_upcast->get_side_center_with_offset(DOWN) + top_to_center_vector;

        Point center = (center_1+center_2)/2;

        center_x = center.x;
        center_y = center.y;
    }

    Point new_center_point = Point(center_x,center_y);

    // matrix to insert in the new puzzle
    Mat to_paste;
    cvtColor(this_piece_cast->get_image(),to_paste,COLOR_GRAY2BGR);
    to_paste = to_paste!=0;
    floodFill(to_paste,this_piece_cast->get_center(),get_random_color());


    // pasting the piece in to the image
    paste_on_top(
            to_paste,
            image,
            this_piece_cast->get_center(),
            new_center_point,
            true
    );

    // updating the position of the piece
    this_piece_cast->set_offset(new_center_point-this_piece_cast->get_center());

    // base case: i need to place a normal piece

}

cv::Scalar PieceArray::get_random_color() {
    // HSV random color
    Scalar color = Scalar (rand()%256, 255, 255);
    Mat in = Mat(Size(1,1),CV_8UC3,color);
    Mat out;
    cvtColor(in,out,COLOR_HSV2BGR);
    int b = out.at<Vec3b>(0, 0)[0];
    int g = out.at<Vec3b>(0, 0)[1];
    int r = out.at<Vec3b>(0, 0)[2];

    Scalar new_color = Scalar(b,g,r);
    cout << new_color << endl;
    return new_color;
}

std::ostream& operator<<(std::ostream& os, const PieceArray& pa){
    int dim_x = pa.get_dim_x();
    int dim_y = pa.get_dim_y();

    for(int y = 0; y<dim_y; y++){
        for(int x = 0; x<dim_x; x++){
            os << pa.get(x,y)->get_debug_view() << " ";
        }
        os << endl;
    }
    return os;
}

