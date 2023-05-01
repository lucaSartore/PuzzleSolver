//
// Created by luca on 4/30/23.
//

#ifndef PIECECLASS_GROUPEDPIECESHOLDER_H
#define PIECECLASS_GROUPEDPIECESHOLDER_H

#include "GroupedPieces.h"
#include "list"

template<int N>
class GroupedPiecesHolder {
private:
    /// how many differen pieces (non counting same orientation of the same piece) the holder is keeping
    int dimension;
    /// heap allocated array for keeping the piece, first index is the id and go form 0 to dimension-1
    /// second index is the orientation
    GroupedPieces<N> * pieces;
public:
    /// return the piece at the specified index and orientation
    GroupedPieces<N> & get(int index, int orientation);
    /// build the object from a list of grouped pieces
    explicit GroupedPiecesHolder(std::list<GroupedPieces<N>> &list);
    /// copy constructor deleted because of heap allocation
    GroupedPiecesHolder(GroupedPiecesHolder & other) = delete;
    /// destructor
    ~GroupedPiecesHolder();
    /// get the length of the group
    int get_length();
};

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
    for(auto &e: list){
        for(int orientation=0; orientation<4; orientation++){
            get(i,orientation) = e;
            get(i,orientation).rotate_by(orientation);
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

#include "GroupedPiecesHolder.tpp"

#endif //PIECECLASS_GROUPEDPIECESHOLDER_H
