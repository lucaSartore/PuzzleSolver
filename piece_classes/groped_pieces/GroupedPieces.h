//
// Created by luca on 4/20/23.
//

#ifndef PIECECLASS_GROUPEDPIECES_H
#define PIECECLASS_GROUPEDPIECES_H
#include "Shore.h"
#include "../puzzle/PieceArray.h"
#include "../puzzle/Holder.h"
#include "../graphic_piece/PieceShape.h"

#include "../logic_piece/PieceConnection.h"
#include "Direction.h"
#include <set>
// generic implementation of the group
template<int N>
class GroupedPieces {
private:
    /// the weighted average shore of the 4 sub-tile and the connection between them
    Shore shore;
    /// a set that contains all the pieces in this group, is useful to avoid repeating pieces
    std::set<int> ids;
    /// 4 pointers that points to the 4 sub components this group is made of
    /// they are in the order: top-left, top-right, bottom-right, bottom-left
    GroupedPieces<N-1>* sub_components[4];
    /// return the top left component
    inline GroupedPieces<N-1>* get_top_left();
    /// return the top right component
    inline GroupedPieces<N-1>* get_top_right();
    /// return the bottom right component
    inline GroupedPieces<N-1>* get_bottom_right();
    /// return the bottom left component
    inline GroupedPieces<N-1>* get_bottom_left();
    /// set the rop left component
    inline void set_top_left(GroupedPieces<N-1>* new_val);
    /// set the top right component
    inline void set_top_right(GroupedPieces<N-1>*new_val);
    /// set the bottom right component
    inline void set_bottom_right(GroupedPieces<N-1>*new_val);
    /// set the bottom left component
    inline void set_bottom_left(GroupedPieces<N-1>*new_val);
    /// calculate the average shore and update it
    void calculate_shore();
public:
    /// return reference to set of all ids contained in the group
    std::set<int>& get_ids();
    /// compare this group with an other, need to specify the direction t
    Shore compare_to(Direction direction, GroupedPieces<N> &other);
    /// get the shore that this group has overall, the shore represent how much the piece match, and go from 0 to 1
    Shore get_shore();
    /// create a Group of pieces using 4 sub components
    GroupedPieces<N>(GroupedPieces<N-1> *top_left, GroupedPieces<N-1> *top_right, GroupedPieces<N-1> *bottom_right, GroupedPieces<N-1> *bottom_left);
    /// return a piece array that represent this subset of pieces, you need to pass him
    /// an array containing all the pieces loaded
    PieceArray get_piece_array(PieceShape* shapes);
};


// specific implementation of the group of one
template<>
class GroupedPieces<1> {
private:
    /// the weighted average shore of the 4 sub-tile and the connection between them
    Shore shore;
    /// reference to all the piece and their meta data
    PieceConnection* piece;
    /// integer from 0 to 3 thad define the orientation of the current piece
    int orientation;
    /// turns a direction into the matching side of this piece
    int direction_to_side_index(Direction direction);
public:
    /// return set containing the id of the piece
    std::set<int> get_ids();
    /// return the id of the current group
    int get_id();
    /// compare this side, at the specified orientation with the other piece, at his specified Direction
    Shore compare_to(Direction direction, GroupedPieces<1> &other);
    /// get the shore that this group has overall, the shore represent how much the piece match, and go from 0 to 1
    Shore get_shore();
    /// build a piece connection, by giving his original piece
    GroupedPieces<1>(PieceConnection* reference_piece, int orientation_);
    /// return a piece array that represent this subset of pieces, you need to pass him
    /// an array containing all the pieces loaded
    PieceArray get_piece_array(PieceShape* shapes);
};

// include file for correct template generation by the compiler
#include "GroupedPieces.tpp"

#endif //PIECECLASS_GROUPEDPIECES_H
