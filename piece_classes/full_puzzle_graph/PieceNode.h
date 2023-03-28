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
class  SideNode;

/// the side of a puzzle piece, that contains
class SideNode {
private:
    // the index of this side from the prospective of the original image
    int side_index;
    // a reference to the piece this side appertains
    PieceNode *piece;
    // a list pointing to the side this piece is connected to
    std::set<SideNode *> connected_sides;
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
    std::set<SideNode *> right_connections[2];
    // same as above, but for left turns
    std::set<SideNode *> left_connections[2];
public:
    // insert into the side metadata a set of sides pointer that can be reachable form this side at a specified distance, following a specified direction
    // note that a distance of 0 means only 1 "hop" while other wise known as the set contained at `connected_sides`, or the pieces that
    // are directly compatible with this piece
    void insert_as_reachable(std::set<SideNode *> &reachable_sides, int distance, Direction direction);

    // reset all the information about the readable pieces
    void reset_distance_metadata();

    //remove a piece form the connected one
    void remove_connection(SideNode* to_remove);

    // return a aet containing all the reachable pieces following a direction at a specified distance
    std::set<SideNode *> &get_reachable_pieces(int distance, Direction direction);

    // check if a side is reachable or not in a specified distance, with a specified direction
    bool is_reachable(SideNode *side_to_reach, int distance, Direction direction);

    // return reference to the side to the left or right of this side in the same piece
    SideNode &get_side_to(Direction direction);

    // default constructor
    SideNode();

    // parameter constructor
    SideNode(int side_index_,PieceNode *piece_);

    // convert the piece to a string for it to be printed
    std::string to_string();

    // return the piece this side apartains to
    PieceNode &get_original_piece();


};
/// a piece that is part of a graph that represent the entire puzzle
class PieceNode {
private:
    // id of the piece
    int piece_id;
    // array with all the sides the piece has
    SideNode sides[4];
    // mutex for mt application
    std::mutex mut;
public:
    // zero parameter constructor
    PieceNode() = default;
    // constructor from a logic piece, it need the array with all the pieces IN ORDER!
    explicit PieceNode(PieceConnections & connections, PieceNode all_pieces[]);
    // return a side from a specified index
    SideNode &get_side(int index);
    // convert the node to a string for printing
    std::string to_string();
    // return the id of the piece
    int get_id();
    // move constructor (thad dose nto move the mutex)
    PieceNode& operator=(PieceNode&& other);
    // return the mutex of the piece
    std::mutex& get_mutex();
};

std::ostream & operator<<(std::ostream& os, PieceNode & pn);

#endif //PIECECLASS_PIECENODE_H
