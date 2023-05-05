//
// Created by luca on 4/20/23.
//

#include "GroupedPieces.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "grouped_pieces_errors.h"
#include "../util.h"
#include "../constants.h"
extern PieceImage* piece_image_global;



using namespace std;

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_top_left(int recursive_orientation) {
    return sub_components[(0+orientation+recursive_orientation)%4];
}

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_top_right(int recursive_orientation) {
    return sub_components[(1+orientation+recursive_orientation)%4];
}

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_bottom_right(int recursive_orientation) {
    return sub_components[(2+orientation+recursive_orientation)%4];
}

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_bottom_left(int recursive_orientation) {
    return sub_components[(3+orientation+recursive_orientation)%4];
}

template<int N>
Shore GroupedPieces<N>::compare_to(Direction direction, GroupedPieces<N> &other,int recursive_orientation,int recursive_orientation_other){
    Shore s;
    switch (direction) {
        case RIGHT:
            s += this->get_top_right(recursive_orientation)->compare_to(direction,*other.get_top_left(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            // early evaluation to save time
            if(s.get_shore() == 0){
                return s;
            }
            s += this->get_bottom_right(recursive_orientation)->compare_to(direction,*other.get_bottom_left(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            break;
        case LEFT:
            s += this->get_top_left(recursive_orientation)->compare_to(direction,*other.get_top_right(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            // early evaluation to save time
            if(s.get_shore() == 0){
                return s;
            }
            s += this->get_bottom_left(recursive_orientation)->compare_to(direction,*other.get_bottom_right(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            break;
        case UP:
            s += this->get_top_left(recursive_orientation)->compare_to(direction,*other.get_bottom_left(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            // early evaluation to save time
            if(s.get_shore() == 0){
                return s;
            }
            s += this->get_top_right(recursive_orientation)->compare_to(direction,*other.get_bottom_right(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            break;
        case DOWN:
            s += this->get_bottom_left(recursive_orientation)->compare_to(direction,*other.get_top_left(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            // early evaluation to save time
            if(s.get_shore() == 0){
                return s;
            }
            s += this->get_bottom_right(recursive_orientation)->compare_to(direction,*other.get_top_right(recursive_orientation),recursive_orientation+orientation,recursive_orientation_other+other.orientation);
            break;
        default:
            throw std::runtime_error("unknown direction");
    }
    //cout << direction << " " << s << endl;
    //if(direction == UP && s.get_shore() == 0.0){
    //    s = Shore(1);
    //}
    return s;
}


Shore GroupedPieces<1>::compare_to(Direction direction, GroupedPieces<1> &other, int recursive_orientation,int recursive_orientation_other) {


    int this_piece_side = direction_to_side_index(direction,recursive_orientation);
    int other_piece_side = other.direction_to_side_index(-direction,recursive_orientation_other); // NOTE: -direction means opposite direction (-UP == DOWN)

    //cout << "comparing piece: " << get_id() << "-" << this_piece_side << " with piece: " << other.get_id() << "-" << other_piece_side << " direction: " << direction << endl;

    float s = piece->compare(
            this_piece_side,
            other.get_id(),
            other_piece_side
            );
    //cout << "Comparing: " << this->get_id() << " and: " << other.get_id() << " Direction: " << direction << " result: " << s << endl;
    // shore with number = 1 and the percentage as shore
    return Shore(s);
}

template<int N>
std::set<int> &GroupedPieces<N>::get_ids() {
    return ids;
}

template<int N>
GroupedPieces<N>::GroupedPieces(GroupedPieces<N - 1> *top_left, GroupedPieces<N - 1> *top_right, GroupedPieces<N - 1> *bottom_right, GroupedPieces<N - 1> *bottom_left) {

    // make share pointer aren't null
    assert(top_left != nullptr);
    assert(top_right != nullptr);
    assert(bottom_right != nullptr);
    assert(bottom_left != nullptr);

    orientation = 0;

    id = 0;

    //reset default ids set
    ids = {};

    // adding top left ids
    ids.insert(top_left->get_ids().begin(),top_left->get_ids().end());

    // adding top right ids
    ids.insert(top_right->get_ids().begin(),top_right->get_ids().end());
    // check if the different ids are the expected number, otherwise it mean that one piece is duplicated
    // so the top right is not a valid piece
    float expected_pieces = pow(4.0,N-1)*0.5;
    if(ids.size() != std::round(expected_pieces)){
        throw TopRightImpossibleCombination();
    }


    // adding bottom right ids
    ids.insert(bottom_right->get_ids().begin(),bottom_right->get_ids().end());
    // check if the different ids are the expected number, otherwise it mean that one piece is duplicated
    // so the bottom left is not a valid piece
    expected_pieces = pow(4.0,N-1)*0.75;
    if(ids.size() != std::round(expected_pieces)){
        throw BottomRightImpossibleCombination();
    }


    // adding bottom left ids
    ids.insert(bottom_left->get_ids().begin(),bottom_left->get_ids().end());
    // check if the different ids are the expected number, otherwise it mean that one piece is duplicated
    // so the bottom left is not a valid piece
    expected_pieces = pow(4.0,N-1);
    if(ids.size() != std::round(expected_pieces)){
        throw BottomLeftImpossibleCombination();
    }

    // insert the pointers to the respective tiles
    set_bottom_left(bottom_left);
    set_bottom_right(bottom_right);
    set_top_right(top_right);
    set_top_left(top_left);

    calculate_shore();
}

template<>
GroupedPieces<2>::GroupedPieces(GroupedPieces<1> *top_left, GroupedPieces<1> *top_right, GroupedPieces<1> *bottom_right, GroupedPieces<1> *bottom_left) {

    // make share pointer aren't null
    assert(top_left != nullptr);
    assert(top_right != nullptr);
    assert(bottom_right != nullptr);
    assert(bottom_left != nullptr);

    orientation = 0;

    id = 0;

    // make array empty
    ids = {};

    // insert the single data
    ids.insert(top_left->get_id());

    // inserting new component in set
    ids.insert(top_right->get_id());
    // if some components are repeated (aka same id) the combination is invalid
    if(ids.size() != 2){
        throw TopRightImpossibleCombination();
    }

    // inserting new component in set
    ids.insert(bottom_right->get_id());
    if(ids.size() != 3){
        throw BottomRightImpossibleCombination();
    }

    // inserting new component in set
    ids.insert(bottom_left->get_id());
    if(ids.size() != 4){
        throw BottomLeftImpossibleCombination();
    }


    // insert the pointers to the respective tiles
    set_bottom_left(bottom_left);
    set_bottom_right(bottom_right);
    set_top_right(top_right);
    set_top_left(top_left);


    // calculating the avrege shore
    calculate_shore();
}


template<int N>
void GroupedPieces<N>::set_top_left(GroupedPieces<N - 1> *new_val, int recursive_orientation) {
    sub_components[(0+orientation+recursive_orientation)%4] = new_val;
}

template<int N>
void GroupedPieces<N>::set_top_right(GroupedPieces<N - 1> *new_val, int recursive_orientation) {
    sub_components[(1+orientation)%4+recursive_orientation] = new_val;
}

template<int N>
void GroupedPieces<N>::set_bottom_right(GroupedPieces<N - 1> *new_val, int recursive_orientation) {
    sub_components[(2+orientation)%4+recursive_orientation] = new_val;
}

template<int N>
void GroupedPieces<N>::set_bottom_left(GroupedPieces<N - 1> *new_val, int recursive_orientation) {
    sub_components[(3+orientation)%4+recursive_orientation] = new_val;
}


std::set<int> GroupedPieces<1>::get_ids() {
    auto set =  std::set<int>();
    set.insert(get_id());
    return set;
}

template<int N>
Shore GroupedPieces<N>::get_shore() {
    return Shore();
}

Shore GroupedPieces<1>::get_shore() {
    return shore;
}

template<int N>
GroupedPieces<N>::GroupedPieces() {
    shore = Shore(0);
    orientation = 0;
    sub_components[0] = nullptr;
    sub_components[1] = nullptr;
    sub_components[2] = nullptr;
    sub_components[3] = nullptr;
}
GroupedPieces<1>::GroupedPieces() {
    shore = Shore(0);
    orientation = 0;
    piece = nullptr;
}


template<int N>
void GroupedPieces<N>::calculate_shore() {
    // reset the current shore;
    shore = Shore();


    // comparing top border
    shore += get_top_left(0)->compare_to(RIGHT,*get_top_right(0),orientation,orientation);
    // trowing an error if the piece is impossible
    if(shore.get_shore() == 0){
        throw TopRightImpossibleFit();
    }


    // comparing right border
    shore += get_top_right(0)->compare_to(DOWN,*get_bottom_right(0),orientation,orientation);
    // trowing an error if the piece is impossible
    if(shore.get_shore() == 0){
        throw BottomRightImpossibleFit();
    }


    // comparing bottom border
    shore += get_bottom_right(0)->compare_to(LEFT,*get_bottom_left(0),orientation,orientation);
    // trowing an error if the piece is impossible
    if(shore.get_shore() == 0){
        throw BottomLeftImpossibleFit();
    }

    // comparing bottom border
    shore += get_bottom_left(0)->compare_to(UP,*get_top_left(0),orientation,orientation);
    // trowing an error if the piece is impossible
    if(shore.get_shore() == 0){
        throw BottomLeftImpossibleFit();
    }

    // trowing an error if the piece is impossible
    if(shore.get_shore() <= MIN_AVG_SHORE_FOR_GROUP){
        throw AvregeIsToLow();
    }

    // adding the avrege of the 4 sub components
    shore += get_top_left(0)->get_shore();
    shore += get_top_right(0)->get_shore();
    shore += get_bottom_right(0)->get_shore();
    shore += get_bottom_left(0)->get_shore();

    // no need to check again for the shore, since the added shore are for shore higher than the threshold
}


int GroupedPieces<1>::direction_to_side_index(Direction direction, int recursive_orientation) {
    int n;
    switch (direction) {
        case UP:
            n = 0;
            break;
        case RIGHT:
            n = 1;
            break;
        case DOWN:
            n = 2;
            break;
        case LEFT:
            n = 3;
            break;
        default:
            throw std::runtime_error("unknown direction!");
    }
    // sum the 2 with module 4 to do
    return (n+orientation+recursive_orientation)%4;
}

GroupedPieces<1>::GroupedPieces(PieceConnection *reference_piece) {
    assert(reference_piece != nullptr);

    orientation = 0;
    piece = reference_piece;
    id = piece->get_piece_id();

    // shore of 1 with 0 as number (don't affect future calculation)
    shore = Shore();
}


template<int N>
void GroupedPieces<N>::rotate_by(int rotate_by) {
    assert(rotate_by>=0);
    assert(rotate_by<4);
    orientation = (orientation+rotate_by)%4;
}

//void rotate_by(int rotate_by);
//template<>
void GroupedPieces<1>::rotate_by(int rotate_by) {
    assert(rotate_by>=0);
    assert(rotate_by<4);
    orientation = (orientation+rotate_by)%4;
}

template<int N>
int GroupedPieces<N>::get_id() {
    return id;
}

template<int N>
void GroupedPieces<N>::set_id(int new_id) {
    id = new_id;
}

int GroupedPieces<1>::get_id() {
    return id;
}

void GroupedPieces<1>::set_id(int new_id) {
    id = new_id;
}



template<int N>
template<class T>
PieceArray<T> GroupedPieces<N>::get_piece_array(PieceImage *shapes,int recursive_orientation) {
    // getting the four sub array
    PieceArray<T> top_left = std::move(get_top_left(recursive_orientation)->template get_piece_array<T>(shapes,recursive_orientation+orientation));
    PieceArray<T> top_right = std::move(get_top_right(recursive_orientation)->template get_piece_array<T>(shapes,recursive_orientation+orientation));
    PieceArray<T> bottom_left = std::move(get_bottom_left(recursive_orientation)->template get_piece_array<T>(shapes,recursive_orientation+orientation));
    PieceArray<T> bottom_right = std::move(get_bottom_right(recursive_orientation)->template get_piece_array<T>(shapes,recursive_orientation+orientation));

    // summing them in to one sub component
    top_left.attach_right(top_right);
    bottom_left.attach_right(bottom_right);
    top_left.attach_bottom(bottom_left);

    // return final piece
    return std::move(top_left);
}


template<class T>
PieceArray<T> GroupedPieces<1>::get_piece_array(PieceImage *shapes,int recursive_orientation) {
    PieceArray<T> pa = PieceArray<T>();
    T ph = T(&shapes[get_id()], (orientation+recursive_orientation)%4);
    pa.set(0,0,std::move(ph));
    return std::move(pa);
}
