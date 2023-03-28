#include "PieceNode.h"

#include <iostream>
#include <set>
#include <assert.h>

using namespace std;



void SideNode::insert_as_reachable(std::set<SideNode *> &reachable_sides, int distance, Direction direction) {
    const std::lock_guard<std::mutex> lock(piece->get_mutex());

    set<SideNode*>* to_edit = &get_reachable_pieces(distance,direction);

    // inserting the new pointers
    for(auto to_insert: reachable_sides){
        to_edit->insert(to_insert);
    }
}

void SideNode::reset_distance_metadata() {
    const std::lock_guard<std::mutex> lock(piece->get_mutex());

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

std::string SideNode::to_string() {

    std::string s;

    for(auto connected_side: connected_sides){
        s += std::to_string(connected_side->piece->get_id()) + "_" + std::to_string(connected_side->side_index) + " ";
    }

    return s;
}

PieceNode &SideNode::get_original_piece() {
    return *piece;
}

void SideNode::remove_connection(SideNode *to_remove) {
    connected_sides.erase(to_remove);
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
        sides[i] = SideNode(i,&all_pieces[piece_id]);

        // adding the links to the original piece
        set<SideNode*> to_add = set<SideNode*>();

        for(auto id_and_side: connections.get_matching_piece_to_side(i)){
            int id = get<0>(id_and_side);
            int side = get<1>(id_and_side);
            to_add.insert(&(all_pieces[id].get_side(side)));
        }

        // insert pointer at direction 0 (means just attached) in this case the direction doesn't meteor
        sides[i].insert_as_reachable(to_add,0,Direction::LEFT);
    }

}

std::string PieceNode::to_string() {

    std::string s;

    s += "piece with id: " + std::to_string(piece_id) + "\n" +
        "pieces to side 0: " + sides[0].to_string() + "\n" +
        "pieces to side 1: " + sides[1].to_string() + "\n" +
        "pieces to side 2: " + sides[2].to_string() + "\n" +
        "pieces to side 3: " + sides[3].to_string() + "\n";

    return s;
}

int PieceNode::get_id() {
    return piece_id;
}

PieceNode &PieceNode::operator=(PieceNode && other) {

    const std::lock_guard<std::mutex> lock(mut);

    piece_id = other.piece_id;

    for(int i=0; i<4; i++){
        sides [i] = std::move(other.sides[i]);
    }
    return *this;

}

std::mutex &PieceNode::get_mutex() {
    return mut;
}

std::ostream & operator<<(std::ostream& os, PieceNode & pn){
    return os << pn.to_string();
}
