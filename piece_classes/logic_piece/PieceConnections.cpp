#include "PieceConnections.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;


void PieceConnections::save_as_file(string path) {

}

PieceConnections::PieceConnections(string path, int id): PieceConnections() {
    this->became(path,id);
}

void PieceConnections::became(string path, int id) {

}


int PieceConnections::get_piece_id() const{
    return piece_id;
}

Connection &PieceConnections::get_matching_piece_to_side(int side){
    // check that the side is in range
    assert(side >= 0);
    assert(side <= 3);
    return *connections[side];
}

ostream & operator<<(ostream& os, PieceConnections& piece){
    os << piece.to_string();
    return os;
}

string PieceConnections::get_side_as_string(int side) const {
    assert(side>=0);
    assert(side<4);
    string s = string("{");
    for(int i=0; i<number_of_pieces; i++){
        auto elem = connections[i];
        s+=  std::to_string(elem->piece_id) + "_" + std::to_string(elem->side) + ": " +  std::to_string(elem->shore) + ", ";
    }
    s+= "}";
    return std::move(s);
}

void PieceConnections::insert_matching_piece(int side_this_piece, Connection &new_connection) {
    assert(side_this_piece>=0);
    assert(side_this_piece<4);

    assert(new_connection.side>=0);
    assert(new_connection.side<4);

    assert(other_piece_id != piece_id);
    mut.lock();
    mut.unlock();
}

PieceConnections::PieceConnections(int id): PieceConnections() {
    this->became(id);
}
void PieceConnections::became(int id) {
    piece_id = id;
    for(auto e: matching_pieces){
        e = set<tuple<int,int>>();
    }
}
PieceConnections::PieceConnections() {
    piece_id = 0;
    for(auto e: matching_pieces){
        e = set<tuple<int,int>>();
    }
}

string PieceConnections::to_string() {

    string s;

    s += "piece_id: " + std::to_string(piece_id) +
            "\nside 0 neighbor: " + get_side_as_string(0) +
            "\nside 1 neighbor: " + get_side_as_string(1) +
            "\nside 2 neighbor: " + get_side_as_string(2) +
            "\nside 3 neighbor: " + get_side_as_string(3);

    return s;
}

/// testing function for saving file
void test_file_save(){
    PieceConnections p1(10);

    p1.insert_matching_piece(11,1,2);
    p1.insert_matching_piece(12,2,2);
    p1.insert_matching_piece(13,3,0);
    p1.insert_matching_piece(14,1,3);
    p1.insert_matching_piece(15,2,1);
    p1.insert_matching_piece(16,1,0);
    p1.insert_matching_piece(17,2,2);
    p1.insert_matching_piece(18,3,1);
    p1.insert_matching_piece(19,0,0);

    p1.save_as_file(".");

    PieceConnections p2(".",10);

    p2.save_as_file("..");

    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;

    assert(p1.to_string() == p2.to_string());

}


Connection::Connection(int piece_, int side_) {
    assert(side_>=0);
    assert(side_<4);
    assert(piece_>=0);
    assert(piece_<PieceConnections::g)
}
