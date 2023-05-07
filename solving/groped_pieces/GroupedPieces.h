//
// Created by luca on 4/20/23.
//

#ifndef PIECECLASS_GROUPEDPIECES_H
#define PIECECLASS_GROUPEDPIECES_H
#include "Shore.h"
#include "../puzzle_preview/PieceArray.h"
#include "../puzzle_preview/PreviewHolder.h"
#include "../puzzle_preview/ShoringHolder.h"
#include "../puzzle_preview/PieceImage.h"

#include "../logic_piece/PieceConnection.h"
#include "Direction.h"
#include <set>


// generic implementation of the group
template<int N>
class GroupedPieces {
private:
    /// the id that reference this piece. it uniquely identify the current piece between all the pieces with the same level (N)
    /// it doe not make difference for the same piece with the same orientation
    int id;
    /// the weighted average shore of the 4 sub-tile and the connection between them
    Shore shore;
    /// a set that contains all the pieces in this group, is useful to avoid repeating pieces
    std::set<int> ids;
    /// flag to keep track of how match a piece is rotated, go from 0 to 3;
    int orientation;
    /// 4 pointers that points to the 4 sub components this group is made of
    /// they are in the order: top-left, top-right, bottom-right, bottom-left
    GroupedPieces<N-1>* sub_components[4];
    /// return the top left component
    /// the recursive iteration is the iteration of the previous aggregation of 4 pieces
    inline GroupedPieces<N-1>* get_top_left(int recursive_orientation);
    /// return the top right component
    inline GroupedPieces<N-1>* get_top_right(int recursive_orientation);
    /// return the bottom right component
    inline GroupedPieces<N-1>* get_bottom_right(int recursive_orientation);
    /// return the bottom left component
    inline GroupedPieces<N-1>* get_bottom_left(int recursive_orientation);
    /// set the rop left component
    inline void set_top_left(GroupedPieces<N-1>* new_val,int recursive_orientation=0);
    /// set the top right component
    inline void set_top_right(GroupedPieces<N-1>*new_val,int recursive_orientation=0);
    /// set the bottom right component
    inline void set_bottom_right(GroupedPieces<N-1>*new_val,int recursive_orientation=0);
    /// set the bottom left component
    inline void set_bottom_left(GroupedPieces<N-1>*new_val,int recursive_orientation=0);
    /// calculate the average shore and update it
    void calculate_shore();
public:
    /// set the id
    void set_id(int new_id);
    /// get the id
    int get_id();
    /// rotate the current piece by 90 degrees times the argument passed
    void rotate_by(int rotate_by);
    /// return reference to set of all ids contained in the group
    std::set<int>& get_ids();
    /// compare this group with an other, need to specify the direction t
    Shore compare_to(Direction direction, GroupedPieces<N> &other,int recursive_orientation_this,int recursive_orientation_other);
    /// get the shore that this group has overall, the shore represent how much the piece match, and go from 0 to 1
    Shore get_shore();
    /// create a Group of pieces using 4 sub components.
    /// this create the piece by placing all pieces one by one starting from the top left, going clockwise
    /// the function throws an error if one of the piece is impossible (and in that case it tells you witch piece, see errors.h)
    /// or it throws an error if they all match, but the avrege shore si too low.
    GroupedPieces<N>(GroupedPieces<N-1> *top_left, GroupedPieces<N-1> *top_right, GroupedPieces<N-1> *bottom_right, GroupedPieces<N-1> *bottom_left);
    /// return a piece array that represent this subset of pieces, you need to pass him
    /// an array containing all the pieces loaded
    template<class T>
    PieceArray<T> get_piece_array(PieceImage* shapes,int recursive_orientation=0);
    /// default constructor (initialize empty group)
    GroupedPieces<N>();
};


// specific implementation of the group of one
template<>
class GroupedPieces<1> {
private:
    int id;
    /// the weighted average shore of the 4 sub-tile and the connection between them
    Shore shore;
    /// reference to all the piece and their meta data
    PieceConnection* piece;
    /// integer from 0 to 3 thad define the orientation of the current piece
    int orientation;
    /// turns a direction into the matching side of this piece
    int direction_to_side_index(Direction direction,int recursive_orientation);
public:
    /// rotate the current piece by 90 degrees times the argument passed
    void rotate_by(int rotate_by);
    /// return set containing the id of the piece
    std::set<int> get_ids();
    /// set the id
    void set_id(int new_id);
    /// get the id
    int get_id();
    /// compare this side, at the specified orientation with the other piece, at his specified Direction
    Shore compare_to(Direction direction, GroupedPieces<1> &other,int recursive_orientation,int recursive_orientation_other);
    /// get the shore that this group has overall, the shore represent how much the piece match, and go from 0 to 1
    Shore get_shore();
    /// build a piece connection, by giving his original piece
    explicit GroupedPieces<1>(PieceConnection* reference_piece);
    /// return a piece array that represent this subset of pieces, you need to pass him
    /// an array containing all the pieces loaded
    template<class T>
    PieceArray<T> get_piece_array(PieceImage* shapes,int recursive_orientation=0);
    /// default constructor, initialize empty group
    GroupedPieces<1>();
};



// include file for correct template generation by the compiler
#include "GroupedPieces.tpp"

#endif //PIECECLASS_GROUPEDPIECES_H
