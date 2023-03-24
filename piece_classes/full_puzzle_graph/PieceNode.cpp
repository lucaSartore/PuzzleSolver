#include "PieceNode.h"

#include <iostream>
#include <set>
#include <assert.h>

using namespace std;



void SideNode::insert_as_reachable(std::set<SideNode *> &reachable_sides, int distance, Direction direction) {

    assert(distance >= 0);
    assert(distance <= 2);

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
}
