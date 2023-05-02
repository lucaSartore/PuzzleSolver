//
// Created by luca on 4/30/23.
//

#include "GroupedPiecesHolder.h"


template<int N>
int GroupedPiecesHolder<N>::get_length() {
    return dimension;
}

template<int N>
GroupedPiecesHolder<N>::~GroupedPiecesHolder() {
    delete[] pieces;
}


template<int N>
GroupedPiecesHolder<N>::GroupedPiecesHolder(std::list<GroupedPieces<N>> &list) {
    // calcolate size
    dimension = list.size();

    // allocate the space in memory for the size in the list
    pieces = new GroupedPieces<N>[dimension*4];

    // creating the list, and making the piece rotated
    int i=0;
    for(auto &element: list){
        for(int orientation=0; orientation<4; orientation++){
            // setting id to the element for easyer identification
            element.set_id(i);
            // creating a new element, rotating it, ans inert it in the new array
            auto new_element = element;
            new_element.rotate_by(orientation);
            get(i,orientation) = new_element;
        }
        i++;
    }

}

template<int N>
GroupedPieces<N> &GroupedPiecesHolder<N>::get(int index, int orientation) {

    assert(index>=0);
    assert(orientation>=0);
    assert(index<dimension);
    assert(orientation<4);

    return pieces[index*4+orientation];
}