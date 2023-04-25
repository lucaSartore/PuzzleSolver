#include "PieceArray.h"
#include "stdexcept"
#include <memory>
#include <utility>
using namespace std;
using namespace cv;


/// how many pixels the image will grow when it runs out of real estate for the new pieces
#define GROWTH_CONSTANT 1000
/// the original dimension of the image
#define STARTING_DIMENSIONS 5000
/// the distance between the pieces and the borders
#define BORDER_DISTANCE 250
/// the number of pixels the fo margin there has to be before the image resolution is increase
#define MARGIN_BEFORE_GROWTH 1000

PieceArray::PieceArray() {
    has_been_completed = false;

    dim_x = 1;
    dim_y = 1;
    pieces = vector<vector<Holder>>();

    // start with a 1*1 array with a null ptr inside
    auto one_dim = vector<Holder>();
    // insert a default Holder
    one_dim.emplace_back();
    pieces.push_back(one_dim);

    image = Mat::zeros(Size(STARTING_DIMENSIONS,STARTING_DIMENSIONS),CV_8UC3);

    //srand(time(NULL));
}

Holder *PieceArray::get(int x, int y){
    // check if in range
    try{
        check_indexes(x,y);
    }catch (...){
        return nullptr;
    }
    auto to_return = &pieces[x][y];

    // if the upper piece is uninitialized i return a null pointer
    if(to_return->get_piece() == nullptr){
        return nullptr;
    }
    return to_return;
}

void PieceArray::set(int x, int y, Holder &&to_be_set) {


    // check if in range
    check_indexes(x,y);

    // updating is completed if we are at the last step!
    if(x==dim_x-1 && y == dim_y-1){
        has_been_completed = true;
    }

    // set the random color
    to_be_set.set_color(get_random_color());

    // inserting it in to the array
    pieces[x][y] = to_be_set;

    // expand the image if to small
    check_and_expand_image();


}


void PieceArray::check_indexes(int x, int y) const {
    // check if indexes are in range
    if(x<0 || y<0 || x >= dim_x || y >= dim_y){
        throw std::invalid_argument("index out of range");
    }
}

void PieceArray::grow_x() {
    auto new_colon = vector<Holder>();
    for(int i=0; i<dim_y; i++){
        new_colon.emplace_back();
    }
    pieces.push_back(new_colon);
    dim_x++;
    has_been_completed = false;
}

void PieceArray::grow_y() {
    for(int i=0; i<dim_x; i++){
        pieces[i].emplace_back();
    }
    dim_y++;
    has_been_completed = false;
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


cv::Mat PieceArray::get_preview_image(){
    reset_image();
    return image.clone();
}

void PieceArray::build_preview_image() {
    for(int x=0; x<get_dim_x(); x++){
        for(int y=0; y<get_dim_y(); y++){
            try{
                insert_into_preview_image(x, y);
            }catch (...){
                // once i get an error, i continue with the next iteration of X value
                break;
            };
        }
    }
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

void PieceArray::check_and_expand_image() {

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
        paste_on_top(image,new_image,Point2i(0,im_size.height-1),Point2i(0, new_image.size().height-1));
        // changing the original image
        image = new_image;
    }

    // updating image size
    im_size = image.size();

    // if need to grow the x res
    if(y_1>im_size.height-MARGIN_BEFORE_GROWTH){
        // creating new bigger image
        Mat new_image = Mat::zeros(Size(im_size.width,im_size.height + GROWTH_CONSTANT), CV_8UC3);
        // pasting the old piece on top of the old piece
        paste_on_top(image, new_image,Point2i(0,0),Point2i(0, 0));
        // changing the original image
        image = new_image;
    }
}

void PieceArray::insert_into_preview_image(int x, int y) {
    check_indexes(x,y);

    // get the piece on the top and bottom of the piece i'm trying to place
    Holder* piece_left = get(x-1,y);
    Holder* piece_top = get(x, y - 1);

    Holder* this_piece = get(x,y);

    // vector that go form the right side of the current piece to the center
    Point left_to_center_vector = this_piece->get_center(true) - this_piece->get_side_center(LEFT,true);
    // vector that go form the top side of the current piece to the center
    Point top_to_center_vector = this_piece->get_center(true) - this_piece->get_side_center(UP,true);

    int center_x,center_y;

    // building on top corner
    if(piece_left == nullptr && piece_top == nullptr){
        // default case: i need to place a corner

        // calculating the point of where to put the new piece
        center_x = BORDER_DISTANCE + left_to_center_vector.x;
        center_y = BORDER_DISTANCE + top_to_center_vector.y;
    }
    // building on the left side
    else if(piece_left == nullptr){
        // default case: i need place a border vertically

        center_x = BORDER_DISTANCE + left_to_center_vector.x;
        center_y = (piece_top->get_side_center_with_offset(DOWN,true) + top_to_center_vector).y;
    }
    // building on the top side
    else if(piece_top == nullptr){
        // default case: i need to place a border horizontally

        center_x = (piece_left->get_side_center_with_offset(RIGHT,true) + left_to_center_vector).x;
        center_y = BORDER_DISTANCE + top_to_center_vector.y;
    }
    // building in the middle
    else{

        Point center_1 = piece_left->get_side_center_with_offset(RIGHT,true) + left_to_center_vector;
        Point center_2 = piece_top->get_side_center_with_offset(DOWN,true) + top_to_center_vector;

        Point center = (center_1+center_2)/2;

        center_x = center.x;
        center_y = center.y;
    }

    Point new_center_point = Point(center_x,center_y);

    // matrix to insert in the new puzzle
    Mat to_paste;
    cvtColor(this_piece->get_image_resized(), to_paste, COLOR_GRAY2BGR);
    to_paste = to_paste!=0;
    floodFill(to_paste, this_piece->get_center(true), this_piece->get_color());

    // pasting the piece in to the image
    paste_on_top(
            to_paste,
            image,
            this_piece->get_center(true),
            new_center_point,
            true
    );

    // updating the position of the piece
    this_piece->set_offset(new_center_point - this_piece->get_center(true));

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
    return new_color;
}

void PieceArray::reset_image() {
    image = Mat::zeros(Size(STARTING_DIMENSIONS,STARTING_DIMENSIONS),CV_8UC3);
    build_preview_image();
}

PieceArray::PieceArray(PieceArray &&other) {
    has_been_completed = other.has_been_completed;

    image = std::move(other.image);
    other.image = Mat();

    dim_x = other.dim_x;
    dim_y = other.dim_y;

    pieces = std::move(other.pieces);
    other.pieces = vector<vector<Holder>>();
}

PieceArray::PieceArray(PieceArray &other) {
    has_been_completed = other.has_been_completed;

    image = other.image.clone();

    dim_x = other.dim_x;
    dim_y = other.dim_y;

    pieces = other.pieces;
}

void PieceArray::attach_right(const PieceArray &other) {

    if(dim_y != other.dim_y){
        throw invalid_argument("the 2 pieces MUST have the same y dimension");
    }

    if(!has_been_completed || !other.has_been_completed){
        throw invalid_argument("both array mus be completed before merging them!");
    }

    for(auto y_colum: other.pieces){
        pieces.push_back(y_colum);
        dim_x++;
    }
}

void PieceArray::attach_bottom(const PieceArray &other) {
    if(dim_x != other.dim_x){
        throw invalid_argument("the 2 pieces MUST have the same x dimension");
    }

    if(!has_been_completed || !other.has_been_completed){
        throw invalid_argument("both array mus be completed before merging them!");
    }

    for(int i=0; i<dim_x; i++){
        pieces[i].insert(
                pieces[i].end(),
                std::make_move_iterator(other.pieces[i].begin()),
                std::make_move_iterator(other.pieces[i].end())
        );
    }
    dim_y += other.dim_y;
}

std::ostream& operator<<(std::ostream& os, const PieceArray& pa){
    int dim_x = pa.get_dim_x();
    int dim_y = pa.get_dim_y();

    for(int y = 0; y<dim_y; y++){
        for(int x = 0; x<dim_x; x++){
            os << "X" << " ";
        }
        os << endl;
    }
    return os;
}

