#include "PieceConnections.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;

int PieceConnections::number_of_pieces = 0;

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
        s+=  std::to_string(i) + "_" + std::to_string(elem->side) + ": " +  std::to_string(elem->shore) + ", ";
    }
    s+= "}";
    return std::move(s);
}


PieceConnections::PieceConnections(int id): PieceConnections() {
    this->became(id);
}
void PieceConnections::became(int id) {
    mut.lock();

    // insert new piece id
    piece_id = id;

    // reset all the connections
    for(auto c: connections){
        for(int i=0; i<number_of_pieces; i++){
            c[i] = Connection();
        }
    }

    mut.unlock();
}
PieceConnections::PieceConnections() {
    piece_id = 0;
    for(auto c: connections){
        c = new Connection[number_of_pieces];
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

void PieceConnections::set_number_of_pieces(int new_val) {
    number_of_pieces = new_val;
}

int PieceConnections::get_number_of_pieces() {
    return number_of_pieces;
}

void PieceConnections::insert_matching_piece(int side_this_piece, int other_piece_id, const Connection &new_connection) {
    assert(side_this_piece>=0);
    assert(side_this_piece<4);
    assert(other_piece_id>=0);
    mut.lock();
    assert(other_piece_id<number_of_pieces);
    connections[side_this_piece][other_piece_id] = new_connection;
    mut.unlock();
}

PieceConnections::~PieceConnections() {
    for(auto p: connections){
        delete[] p;
    }
}

/// testing function for saving file
void test_file_save(){
    PieceConnections p1(10);


    p1.save_as_file(".");

    PieceConnections p2(".",10);

    p2.save_as_file("..");

    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;

    assert(p1.to_string() == p2.to_string());

}




Connection::Connection(int side_, float shore_) {
    assert(side_>=0);
    assert(side_<4);

    side = side_;
    shore = shore_;
}

Connection::Connection() {
    side = 0;
    shore = -1;
}
