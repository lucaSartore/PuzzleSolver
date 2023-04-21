//
// Created by luca on 4/20/23.
//

#include "GroupedPieces.h"
#include <cmath>
#include <algorithm>

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



float GroupedPieces<1>::get_shore() {
    return 1;
}

float GroupedPieces<1>::compare_to(Direction direction, GroupedPieces<1> &other) {
    return piece->compare(
            direction_to_side_index(direction),
            other.get_id(),
            other.direction_to_side_index(-direction) // note: -direction means opposite direction (-UP == DOWN)
            );
}

template<int N>
std::set<int> &GroupedPieces<N>::get_ids() {
    return ids;
}

template<int N>
GroupedPieces<N>::GroupedPieces(GroupedPieces<N - 1> *top_left, GroupedPieces<N - 1> *top_right, GroupedPieces<N - 1> *bottom_right, GroupedPieces<N - 1> *bottom_left) {

    cout << "AAAAA" << endl;

    /*
    // conacat. all the set
    std::merge(top_left->get_ids().begin(), top_left->get_ids().end(),
               top_right->get_ids().begin(), top_right->get_ids().end(),
               bottom_right->get_ids().begin(), bottom_right->get_ids().end(),
               bottom_left->get_ids().begin(), bottom_left->get_ids().end(),
               std::inserter(ids, ids.begin()));

    // check if the different ids are the expected number, otherwise it mean that one piece is duplicated, so this is not a valid piece
    float expected_pieces = pow(4.0,N-1);

    cout << "subs: " << endl;
    for(auto e: ids){
        cout << e << ", ";
    }
    cout << endl;

    if(ids.size() != std::round(expected_pieces)){
        throw std::invalid_argument("some pieces are repeated");
    }*/
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
