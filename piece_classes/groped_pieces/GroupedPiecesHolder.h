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

extern PieceImage* piece_image_global;

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
            // creating a new element, rotating it, ans inert it in the new array
            auto new_element = element;
            new_element.rotate_by(orientation);
            get(i,orientation) = new_element;

            /*
            if(N == 2){
                auto pa = get(i,orientation).template get_piece_array<PreviewHolder>(piece_image_global);
                imshow("new_element",pa.get_preview_image());

                if(orientation>0){
                    auto pa = get(i,orientation-1).template get_piece_array<PreviewHolder>(piece_image_global);
                    imshow("prev",pa.get_preview_image());
                }
                waitKey(0);
            }*/
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
