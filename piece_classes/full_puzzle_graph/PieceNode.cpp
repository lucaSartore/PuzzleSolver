#include "PieceNode.h"

#include <iostream>
#include <set>
#include <assert.h>

using namespace std;



void SideNode::insert_as_reachable(std::set<SideNode *> &reachable_sides, int distance, Direction direction) {

    set<SideNode*>* to_edit = &get_reachable_pieces(distance,direction);

    // inserting the new pointers
    for(auto to_insert: reachable_sides){
        to_edit->insert(to_insert);
    }
}

void SideNode::reset_distance_metadata() {
    right_connections[0] = set<SideNode*>();
    right_connections[1] = set<SideNode*>();
    left_connections[0] = set<SideNode*>();
    left_connections[1] = set<SideNode*>();
}

std::set<SideNode *> &SideNode::get_reachable_pieces(int distance, Direction direction) {
    assert(distance >= 0);
    assert(distance <= 2);
    // find out witch array i need to edit
    set<SideNode*>* to_edit;
    if(distance == 0){
        to_edit = &connected_sides;
    }else{
        if(direction == Direction::LEFT){
            to_edit = & left_connections[distance-1];
        }else{
            to_edit = & right_connections[distance-1];
        }
    }
    return *to_edit;
}

bool SideNode::is_reachable(SideNode *side_to_reach, int distance, Direction direction) {

    set<SideNode*> reachable = get_reachable_pieces(distance,direction);
    return reachable.count(side_to_reach);
}

SideNode &SideNode::get_side_to(Direction direction) {
    int index_to_return = side_index;
    if(direction == Direction::LEFT){
        index_to_return += 1;
        index_to_return %= 4;
    }else{
        index_to_return -=1;
        if(index_to_return < 0){
            index_to_return += 4;
        }
    }
    return piece->get_side(index_to_return);
}

SideNode::SideNode() {
    piece = nullptr;
    side_index = 0;
}

SideNode::SideNode(int side_index_, PieceNode *piece_) {
    piece = piece_;
    side_index = side_index_;
    connected_sides = set<SideNode*>();
    for(int i=0; i<2; i++){
        right_connections[i] = set<SideNode*>();
        left_connections[i] = set<SideNode*>();
    }
}

SideNode &PieceNode::get_side(int index) {
    assert(index>=0);
    assert(index<4);
    return sides[index];
}

PieceNode::PieceNode(PieceConnections &connections, PieceNode all_pieces[]) {

    piece_id = connections.get_piece_id();

    for(int i=0; i<4; i++){
        // creating the side object
        sides[i] = SideNode(i,this);

        // adding the links to the original piece
        set<SideNode*> to_add = set<SideNode*>();

        // do do: the connection must be refered to bouth sides, and not only one!

        // indering the reference to each
        for(auto id: connections.get_matching_piece_to_side(i)){

        }

    }

}

