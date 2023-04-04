#include "PieceConnections.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;


void PieceConnections::save_as_file(string path) {
    mut.lock();
    path += "/" + std::to_string(piece_id)+".txt";
    ofstream file(path);
    if (file.is_open()) {
        file << piece_id << endl; // write piece id to file
        for (int i = 0; i < 4; i++) {
            for (auto& tuple : matching_pieces[i]) {
                file << get<0>(tuple) << " " << get<1>(tuple) << " ";
            }
            file << endl;
        }
        file.close();
    }else{
        cerr << "impossible to open file: " << path << endl;
    }
    mut.unlock();
}

PieceConnections::PieceConnections(string path, int id): PieceConnections() {
    this->became(path,id);
}

void PieceConnections::became(string path, int id) {
    mut.lock();
    path += "/" + std::to_string(id) + ".txt";
    ifstream file(path);
    if (file.is_open()) {
        file >> piece_id;
        string line;
        getline(file, line);
        for (auto & matching_piece : matching_pieces) {
            int a,b;
            getline(file, line);

            istringstream iss(line);

            while (iss >> a >> b) {
                matching_piece.insert(make_tuple(a, b));
            }

        }
        file.close();    } else {
        cerr << "Error: could not open file " << path << endl;
    }
    mut.unlock();
}


int PieceConnections::get_piece_id() const{
    return piece_id;
}

set<tuple<int,int>> &PieceConnections::get_matching_piece_to_side(int side){
    // check that the side is in range
    assert(side >= 0);
    assert(side <= 3);
    return matching_pieces[side];
}

ostream & operator<<(ostream& os, PieceConnections& piece){

    os << piece.to_string();

    return os;
}

string PieceConnections::get_side_as_string(int side) const {
    assert(side>=0);
    assert(side<4);
    string s = string("{");
    for(auto e: matching_pieces[side]){
        s+= std::to_string(get<0>(e)) + "_" + std::to_string(get<1>(e)) + ", ";
    }
    s+= "}";
    return std::move(s);
}

void PieceConnections::insert_matching_piece(int other_piece_id,int side_this_piece, int side_other_piece) {
    assert(side_this_piece>=0);
    assert(side_this_piece<4);

    assert(side_other_piece>=0);
    assert(side_other_piece<4);

    assert(other_piece_id != piece_id);
    mut.lock();
    matching_pieces[side_this_piece].insert(tuple<int,int>(other_piece_id,side_other_piece));
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


