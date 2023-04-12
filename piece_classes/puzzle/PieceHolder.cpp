//
// Created by luca on 4/4/23.
//

#include "PieceHolder.h"

// if a border gets compared with an outside piece, this is the compatibility value that it will get
#define BORDER_COMPATIBILITY 0.6


char PieceHolder::get_debug_view() const{
    return 'P';
}


// the function return the average of the compatibility of the know pieces
// it return 0 if the possibility of a compatibility is 0, for example if we have a male-male cameraperson
float PieceHolder::check_compatibility(Holder *up, Holder *down, Holder *left, Holder *right){


    Direction directions[] = {UP,DOWN,RIGHT,LEFT};
    Holder* holders[] = {up,down,right,left};


    int count=0;
    float average=0;

    for(int i=0; i<4; i++){

        float compatibility;

        // the direction i'm comparing to
        Direction direction = directions[i];
        // the holder i'm comparing to
        Holder* to_compare = holders[i];

        // if the piece is unknown i skip it
        if(to_compare->is_unknown()){
            continue;
        }

        // the side of this piece i am comparing
        Side* my_side = this->get_side(direction);
        // the side of the other piece i am comparing
        Side* his_side = to_compare->get_side(-direction);

        // if i'm comparing with an outside piece, the piece is compatible only if
        // he is a side
        if(to_compare->is_outside()){
            if(my_side->get_kind() != SideKind::BORDER){
                return  0.0;
            }else{
                average += 0.6;
                count++;
            }
        }else{
            // otherwise the side is normal, and so i do to the normal compare between pieces
            compatibility = my_side->compare_to(*his_side);
            if(compatibility == 0.0){
                return 0.0;
            }
            average += (float)compatibility;
            count++;
        }
    }
    if(count == 0){
        throw std::runtime_error("all sides are unknown!");
    }
    // return the average of the
    return  average/((float)count);
}

Side *PieceHolder::get_side(Direction direction) {
    int offset;
    switch (direction) {
        case UP:
            offset=0;
            break;
        case RIGHT:
            offset=1;
            break;
        case DOWN:
            offset=2;
            break;
        case LEFT:
            offset=3;
            break;
        default:
            throw std::runtime_error("unknown direction");
    }
    return &piece->get_side((orientation+offset)%4);
}

PieceHolder::PieceHolder(Piece *piece_, int orientation_): Holder(){
    assert(piece_ != nullptr);
    assert(orientation_>=0 && orientation_<4);

    piece = piece_;
    orientation = orientation_;
}

bool PieceHolder::is_a_piece() {
    return true;
}

cv::Mat PieceHolder::get_image() {
    int rotate_code;
    switch (orientation) {
        case 0:
            // no need for rotation;
            return piece->get_image().clone();
        case 1:
            rotate_code = cv::ROTATE_90_COUNTERCLOCKWISE;
            break;
        case 2:
            rotate_code = cv::ROTATE_180;
            break;
        case 3:
            rotate_code = cv::ROTATE_90_CLOCKWISE;
            break;
        default:
            throw std::runtime_error("unknown orientation");
    }
    cv::Mat to_return;
    cv::rotate(piece->get_image(),to_return,rotate_code);
    return to_return;
}

cv::Point PieceHolder::get_side_center(Direction direction) {
    int offset;
    switch (direction) {
        case UP:
            offset=0;
            break;
        case RIGHT:
            offset=1;
            break;
        case DOWN:
            offset=2;
            break;
        case LEFT:
            offset=3;
            break;
        default:
            throw std::runtime_error("unknown direction");
    }
    cv::Point p1 = get_point((offset)%4);
    cv::Point p2 = get_point((offset+1)%4);
    return (p1+p2)/2;
}

cv::Point PieceHolder::get_point(int index) {
    assert(index>=0);
    assert(index<4);
    cv::Point og_point = piece->get_point((orientation+index)%4);
    int res_x = piece->get_preview_x_res();
    int res_y = piece->get_preview_y_res();
    int x,y;
    switch (orientation) {
        case 0:
            // no rotation
            x = og_point.x;
            y = og_point.y;
            break;
        case 1:
            // 90 deg counter clock wise
            x = og_point.y;
            y = res_y-og_point.x;
            break;
        case 2:
            // 180 deg
            x = res_x-og_point.x;
            y = res_y-og_point.y;
            break;
        case 3:
            // 90 deg clock wise
            x = res_x-og_point.y;
            y = og_point.x;
            break;
        default:
            throw std::runtime_error("unknown orientation");
    }
    return cv::Point(x,y);

}

cv::Point PieceHolder::get_side_center_with_offset(Direction direction) {
    return this->get_side_center(direction) + this->get_offset();
}

cv::Point PieceHolder::get_center() {
    cv::Point p1 = get_point(0);
    cv::Point p2 = get_point(1);
    cv::Point p3 = get_point(2);
    cv::Point p4 = get_point(3);
    return (p1+p2+p3+p4)/4;
}
