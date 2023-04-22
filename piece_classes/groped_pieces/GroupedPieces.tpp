//
// Created by luca on 4/20/23.
//

#include "GroupedPieces.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

// the threshold after witch a component get removed because of poor connections
#define AVREGE_SHORE_THRESHOLD 0.7

using namespace std;

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_top_left() {
    return sub_components[0];
}

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_top_right() {
    return sub_components[1];
}

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_bottom_right() {
    return sub_components[2];
}

template<int N>
GroupedPieces<N - 1> *GroupedPieces<N>::get_bottom_left() {
    return sub_components[3];
}


Shore GroupedPieces<1>::compare_to(Direction direction, GroupedPieces<1> &other) {
    float s = piece->compare(
            direction_to_side_index(direction),
            other.get_id(),
            other.direction_to_side_index(-direction) // note: -direction means opposite direction (-UP == DOWN)
            );
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

    //reset default ids set
    ids = {};

    // concat all the sets
    std::set<int> t1 = {},t2 = {};
    std::set_union(top_left->get_ids().begin(), top_left->get_ids().end(),
               top_right->get_ids().begin(), top_right->get_ids().end(),
               std::inserter(t1, t1.begin()));
    std::set_union(bottom_right->get_ids().begin(), bottom_right->get_ids().end(),
                   bottom_left->get_ids().begin(), bottom_left->get_ids().end(),
                   std::inserter(t2, t2.begin()));
    std::set_union(t1.begin(), t1.end(),
                   t2.begin(), t2.end(),
                   std::inserter(ids, ids.begin()));

    // check if the different ids are the expected number, otherwise it mean that one piece is duplicated, so this is not a valid piece
    float expected_pieces = pow(4.0,N-1);
    if(ids.size() != std::round(expected_pieces)){
        throw std::invalid_argument("some pieces are repeated");
    }

    // insert the pointers to the respective tiles
    set_bottom_left(bottom_left);
    set_bottom_right(bottom_right);
    set_top_right(top_right);
    set_top_left(top_left);
}

template<>
GroupedPieces<2>::GroupedPieces(GroupedPieces<1> *top_left, GroupedPieces<1> *top_right, GroupedPieces<1> *bottom_right, GroupedPieces<1> *bottom_left) {

    // make share pointer aren't null
    assert(top_left != nullptr);
    assert(top_right != nullptr);
    assert(bottom_right != nullptr);
    assert(bottom_left != nullptr);

    // make array empty
    ids = {};

    // insert the single data
    ids.insert(top_left->get_id());
    ids.insert(top_right->get_id());
    ids.insert(bottom_right->get_id());
    ids.insert(bottom_left->get_id());

    // check if all ids are different, otherwise throw an error
    if(ids.size() != 4){
        throw std::invalid_argument("some pieces are repeated");
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
void GroupedPieces<N>::set_top_left(GroupedPieces<N - 1> *new_val) {
    sub_components[0] = new_val;
}

template<int N>
void GroupedPieces<N>::set_top_right(GroupedPieces<N - 1> *new_val) {
    sub_components[1] = new_val;
}

template<int N>
void GroupedPieces<N>::set_bottom_right(GroupedPieces<N - 1> *new_val) {
    sub_components[2] = new_val;
}

template<int N>
void GroupedPieces<N>::set_bottom_left(GroupedPieces<N - 1> *new_val) {
    sub_components[3] = new_val;
}



int GroupedPieces<1>::get_id() {
    return piece->get_piece_id();
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
void GroupedPieces<N>::calculate_shore() {
    // reset the current shore;
    shore = Shore();

    // comparing top border
    shore += get_top_left()->compare_to(RIGHT,*get_top_right());
    // comparing right border
    shore += get_top_right()->compare_to(DOWN,*get_bottom_right());
    // comparing bottom border
    shore += get_bottom_right()->compare_to(LEFT,*get_bottom_left());
    // comparing bottom border
    shore += get_bottom_left()->compare_to(UP,*get_top_left());

    // trowing an error if the piece is impossible
    if(shore.get_shore() == 0){
        throw invalid_argument("impossible combination");
    }
    // trowing an error if the piece is impossible
    if(shore.get_shore() <= AVREGE_SHORE_THRESHOLD){
        throw invalid_argument("combination shore too low");
    }

    // adding the avrege of the 4 sub components
    shore += get_top_left()->get_shore();
    shore += get_top_left()->get_shore();
    shore += get_top_left()->get_shore();
    shore += get_top_left()->get_shore();

    // trowing an error if the piece is impossible
    if(shore.get_shore() == 0){
        throw invalid_argument("impossible combination");
    }
    // trowing an error if the piece is impossible
    if(shore.get_shore() <= AVREGE_SHORE_THRESHOLD){
        throw invalid_argument("combination shore too low");
    }

}


int GroupedPieces<1>::direction_to_side_index(Direction direction) {
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
    return (n+orientation)%4;
}

GroupedPieces<1>::GroupedPieces(PieceConnection *reference_piece, int orientation_) {
    assert(orientation_>=0);
    assert(orientation_<4);
    assert(reference_piece != nullptr);

    orientation = orientation_;
    piece = reference_piece;

    // shore of 1 with 0 as number (don't affect future calculation)
    shore = Shore();
}
