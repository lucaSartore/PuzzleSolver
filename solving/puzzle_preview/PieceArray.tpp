#include "PieceArray.h"
#include "stdexcept"
#include "ShoringHolder.h"
#include <memory>
#include <utility>
#include "point_indexes.h"

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


/// the amount of pixels a piece will be dilated and eroded in order to remove the cracks
#define DILAT_EROS_CRACKS_REMOVAL 5

template<class T>
PieceArray<T>::PieceArray() {
    has_been_completed = false;

    dim_x = 1;
    dim_y = 1;
    pieces = vector<vector<T>>();

    // start with a 1*1 array with an empty holder inside
    auto one_dim = vector<T>();
    // insert a default Holder
    one_dim.emplace_back();
    pieces.push_back(one_dim);

    image = Mat::zeros(Size(STARTING_DIMENSIONS,STARTING_DIMENSIONS),CV_8UC3);

    //srand(time(NULL));
}
template<class T>
T *PieceArray<T>::get(int x, int y){
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

template<class T>
void PieceArray<T>::set(int x, int y, T &&to_be_set) {
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

template<class T>
void PieceArray<T>::check_indexes(int x, int y) const {
    // check if indexes are in range
    if(x<0 || y<0 || x >= dim_x || y >= dim_y){
        throw std::invalid_argument("index out of range");
    }
}

template<class T>
void PieceArray<T>::grow_x() {
    auto new_colon = vector<T>();
    for(int i=0; i<dim_y; i++){
        new_colon.emplace_back();
    }
    pieces.push_back(new_colon);
    dim_x++;
    has_been_completed = false;
}

template<class T>
void PieceArray<T>::grow_y() {
    for(int i=0; i<dim_x; i++){
        pieces[i].emplace_back();
    }
    dim_y++;
    has_been_completed = false;
}

template<class T>
int PieceArray<T>::get_dim_x() const{
    return dim_x;
}

template<class T>
int PieceArray<T>::get_dim_y() const  {
    return dim_y;
}

template<class T>
void PieceArray <T>::un_grow_x() {
    if(dim_x != 0){
        pieces.pop_back();
        dim_x--;
    }
}

template<class T>
void PieceArray<T>::un_grow_y() {
    if(dim_y != 0){
        for(int i=0; i<dim_x; i++){
            pieces[i].pop_back();
        }
        dim_y--;
    }
}

template<class T>
cv::Mat PieceArray<T>::get_preview_image(){
    assert(has_been_completed);
    reset_image();
    return image.clone();
}

template<class T>
void PieceArray<T>::build_image() {
    for(int x=0; x<get_dim_x(); x++){
        for(int y=0; y<get_dim_y(); y++){
            try{
                insert_into_image(x, y);
                check_and_expand_image();
            }catch (...){
                // once i get an error, i continue with the next iteration of X value
                break;
            };
        }

    }
}

template<class T>
void PieceArray<T>::paste_on_top(const cv::Mat& source, cv::Mat& destination, cv::Point2i pointSource, cv::Point2i pointDestination,PateOnTopMethod method) {

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


    Mat result;

    switch (method) {
        case OR:
            cv::bitwise_or(source,croppedDestination,result);
            break;
        case SUM:
            result = source+croppedDestination;
            break;
        case XOR:
            cv::bitwise_xor(source,croppedDestination,result);
            break;
        case OVERWRITE:
            result = source;
            break;
        default:
            throw runtime_error("unknown method");
    }

    result.copyTo(croppedDestination);
}

template<class T>
void PieceArray<T>::check_and_expand_image() {

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
        paste_on_top(image,new_image,Point2i(0,im_size.height-1),Point2i(0, new_image.size().height-1),OVERWRITE);
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
        paste_on_top(image, new_image,Point2i(0,0),Point2i(0, 0),OVERWRITE);
        // changing the original image
        image = new_image;
    }
}

template<>
void PieceArray<PreviewHolder>::insert_into_image(int x, int y) {
    check_indexes(x,y);

    // get the piece on the top and bottom of the piece i'm trying to place
    PreviewHolder* piece_left = get(x - 1, y);
    PreviewHolder* piece_top = get(x, y - 1);

    PreviewHolder* this_piece = get(x, y);

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

    // matrix to insert in the new puzzle_preview
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
            OR
    );

    // updating the position of the piece
    this_piece->set_offset(new_center_point - this_piece->get_center(true));
}

template<>
void PieceArray<ShoringHolder>::insert_into_image(int x, int y) {
    check_indexes(x,y);

    // get the piece on the top and bottom of the piece i'm trying to place
    ShoringHolder* piece_left = get(x - 1, y);
    ShoringHolder* piece_top = get(x, y - 1);

    ShoringHolder* this_piece = get(x, y);

    int center_x,center_y;

    // building on top corner
    if(piece_left == nullptr && piece_top == nullptr){
        // default case: i need to place a corner

        // vector that go form the right side of the current piece to the center
        Point left_to_center_vector = this_piece->get_center(true) - this_piece->get_side_center(LEFT,true);
        // vector that go form the top side of the current piece to the center
        Point top_to_center_vector = this_piece->get_center(true) - this_piece->get_side_center(UP,true);

        // calculating the point of where to put the new piece
        center_x = BORDER_DISTANCE + left_to_center_vector.x;
        center_y = BORDER_DISTANCE + top_to_center_vector.y;

        this_piece->set_offset(Point(center_x,center_y));
    }
        // building on the left side
    else if(piece_left == nullptr){
        this_piece->align_to(*piece_top,UP);
        this_piece->move_to(*piece_top,UP);
    }
        // building on the top side
    else if(piece_top == nullptr){
        this_piece->align_to(*piece_left,LEFT);
        this_piece->move_to(*piece_left,LEFT);
    }
        // building in the middle
    else{
        this_piece->align_to(*piece_top,*piece_left);
        this_piece->move_to(*piece_top,*piece_left);
    }


    Point new_center_point = this_piece->get_rotated_center_with_offset();


    // matrix to insert in the new puzzle_preview
    Mat to_paste;
    cvtColor(this_piece->get_rotated_image(), to_paste, COLOR_GRAY2BGR);
    to_paste = to_paste!=0;

    // pasting the piece in to the image
    paste_on_top(
            to_paste,
            image,
            this_piece->get_center(false),
            new_center_point,
            XOR
    );


}


template<class T>
cv::Scalar PieceArray<T>::get_random_color() {
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

template<class T>
void PieceArray<T>::reset_image() {
    image = Mat::zeros(Size(STARTING_DIMENSIONS,STARTING_DIMENSIONS),CV_8UC3);
    build_image();
}

template<class T>
PieceArray<T>::PieceArray(PieceArray<T> &&other) {
    has_been_completed = other.has_been_completed;

    image = std::move(other.image);
    other.image = Mat();

    dim_x = other.dim_x;
    dim_y = other.dim_y;

    pieces = std::move(other.pieces);
    other.pieces = vector<vector<T>>();
}
template<class T>
PieceArray<T>::PieceArray(PieceArray<T> &other) {
    has_been_completed = other.has_been_completed;

    image = other.image.clone();

    dim_x = other.dim_x;
    dim_y = other.dim_y;

    pieces = other.pieces;
}

template<class T>
void PieceArray<T>::attach_right(const PieceArray<T> &other) {

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

template<class T>
void PieceArray<T>::attach_bottom(const PieceArray<T> &other) {
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


template<>
float PieceArray<ShoringHolder>::get_shore() {
    // calculating the image
    reset_image();

    // creating gray mask
    Mat img_gray;
    cvtColor(image,img_gray,COLOR_BGR2GRAY);
    img_gray = img_gray > 0;

    // kernel for erosion and dilatations
    Mat kernel = Mat::ones(DILAT_EROS_CRACKS_REMOVAL,DILAT_EROS_CRACKS_REMOVAL*2,CV_8U) == 1;

    // expanding and eroding the black mask to get a version with no craks
    Mat gray_with_no_cracks,temp;
    dilate(img_gray,temp,kernel);
    erode(temp,gray_with_no_cracks,kernel);

    // mask containing pieces that are outside of the considered area;
    Mat outside_mask = gray_with_no_cracks.clone();
    floodFill(outside_mask,Point(0,0),44);
    outside_mask = outside_mask == 44;

    // matrix showing where pieces dose not fit;
    Mat errors;
    bitwise_xor(img_gray,outside_mask,errors);

    // image containing a "share" delimited by the 4 external points
    Mat square_mask = Mat::zeros(img_gray.size(),CV_8U);

    Point p1,p2;

    p1 = get(0,0)->get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER);
    p2 = get(dim_x-1,0)->get_rotated_point_with_offset(TOP_RIGHT_PIECE_CORNER);
    line(square_mask,p1,p2,Scalar(255),3);

    p1 = get(dim_x-1,dim_y-1)->get_rotated_point_with_offset(BOTTOM_RIGHT_PIECE_CORNER);
    line(square_mask,p1,p2,Scalar(255),3);

    p2 = get(0,dim_y-1)->get_rotated_point_with_offset(BOTTOM_LEFT_PIECE_CORNER);
    line(square_mask,p1,p2,Scalar(255),3);

    p1 = get(0,0)->get_rotated_point_with_offset(TOP_LEFT_PIECE_CORNER);
    line(square_mask,p1,p2,Scalar(255),3);


    floodFill(square_mask,Point(0,0),255);


    // removing black pixels thad end up been outside of the 4 cornners
    bitwise_or(errors,square_mask,temp);
    errors = temp;

    //Mat resized;resize(errors,resized,errors.size()/8);imshow("errors", resized);waitKey(0);

    int bad_pixels = countNonZero(errors==0);
    int total_pixels = countNonZero(square_mask==0);

    return (float)(total_pixels-bad_pixels)/(float)total_pixels;
}
