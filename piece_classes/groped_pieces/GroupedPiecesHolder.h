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



#include "GroupedPiecesHolder.tpp"

#endif //PIECECLASS_GROUPEDPIECESHOLDER_H
