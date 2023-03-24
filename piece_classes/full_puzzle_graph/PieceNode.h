//
// Created by luca on 3/23/23.
//

#ifndef PIECECLASS_PIECENODE_H
#define PIECECLASS_PIECENODE_H


#include <iostream>
#include <set>
#include <list>
#include "../logic_piece/PieceConnections.h"

// direction for the many options to go for
typedef enum Direction{RIGHT,LEFT} Direction;

class PieceNode;
class  Side;

/// the side of a puzzle piece, that contains
class Side {
private:
    // the index of this side from the prospective of the original image
    int side_index;
    // a reference to the piece this side appertains
    PieceNode *piece;
    // a list pointing to the side this piece is connected to
    std::set<Side *> connected_sides;
    // an array of set of connected pieces
    // this store the side that can be linked to this side, by doing only right  right turns, at the distance index -1
    // for example:
    // right_connections[0] represent a set of all the sides, that are readable by jumping 1 time
    // right_connections[3] (if it existed) must contain the pointer to a Side of this piece, otherwise the path is not valid
    //
    // this is used because:
    //      you are piece A, in your side 1 (named A1) you have a pointer to the side X adn side Y, you don't know which
    //      one of them is the correct one, but you know that if you manage to go X, then go to the piece at the right,
    //      repeat this 3 times and you end up in a side of yourself, you hava close the loop, and you know that X is at
    //      least possible a candidate piece. vice-versa, if is impossible for you to reach yourself by visiting 3 pieces
    //      and only turning right form Y, you can discard Y from the possible connections
    std::set<Side *> right_connections[2];
    // same as above, but for left turns
    std::set<Side *> left_connections[2];
public:
    // insert into the side metadata a set of sides pointer that can be reachable form this side at a specified distance, following a specified direction
    // note that a distance of 0 means only 1 "hop" while other wise known as the set contained at `connected_sides`, or the pieces that
    // are directly compatible with this piece
    void insert_as_reachable(std::set<Side *> &reachable_sides, int distance, Direction direction);

    // reset all the information about the readable pieces
    void reset_distance_metadata();

    // return a aet containing all the reachable pieces following a direction at a specified distance
    std::set<Side *> &get_reachable_pieces(int distance, Direction direction);

    // check if a side is reachable or not in a specified distance, with a specified direction
    bool is_reachable(Side *side_to_reach, int distance, Direction direction);

    // return reference to the side to the left or right of this side in the same piece
    Side &get_side_to(Direction direction);

    // default constructor
    Side();

    // parameter constructor
    Side(int side_index_,PieceNode *piece);


};
/// a piece that is part of a graph that represent the entire puzzle
class PieceNode {
private:
    // array with all the sides the piece has
    Side sides[4];
public:
    // zero parameter constructor
    PieceNode();
    // constructor from a logic piece
    explicit PieceNode(PieceConnections && connections);
};


#endif //PIECECLASS_PIECENODE_H
