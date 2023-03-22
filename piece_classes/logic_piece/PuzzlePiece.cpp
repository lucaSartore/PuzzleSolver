#include "PuzzlePiece.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <assert.h>

void PuzzlePiece::save_as_file(string path) {
    path += "/" + to_string(piece_id)+".txt";
    ofstream outfile(path);
    if (outfile.is_open()) {
        outfile << piece_id << endl;

        for (int i = 0; i < 4; i++) {
            ostream_iterator<int> output_iterator(outfile, " ");
            copy(matching_pieces[i].begin(), matching_pieces[i].end(), output_iterator);
            outfile << endl;
        }

        outfile.close();
    } else {
        cerr << "Error: could not open file " << path << endl;
    }
}

PuzzlePiece::PuzzlePiece(string path, int id) {
    path += "/" + to_string(id) + ".txt";
    ifstream infile(path);
    if (infile.is_open()) {
        string line;

        // read piece ID
        getline(infile, line);
        piece_id = stoi(line);

        // read matching pieces
        for (int i = 0; i < 4; i++) {
            getline(infile, line);
            istringstream iss(line);
            int piece;
            while (iss >> piece) {
                matching_pieces[i].insert(piece);
            }
        }

        infile.close();
    } else {
        cerr << "Error: could not open file " << path << endl;
    }
}

int PuzzlePiece::get_piece_id() const{
    return piece_id;
}

set<int> &PuzzlePiece::get_matching_piece_to_side(int side){
    // check that the side is in range
    assert(side >= 0);
    assert(side <= 3);
    return matching_pieces[side];
}

ostream & operator<<(ostream& os, PuzzlePiece& piece){

    os << "piece_id: " << piece.piece_id <<
    "\nside 0 neighbor: " << piece.get_side_as_string(0) <<
    "\nside 1 neighbor: " << piece.get_side_as_string(1) <<
    "\nside 2 neighbor: " << piece.get_side_as_string(2) <<
    "\nside 3 neighbor: " << piece.get_side_as_string(3);

    return os;
}

string PuzzlePiece::get_side_as_string(int side) const {
    assert(side>=0);
    assert(side<4);
    string s = string("{");
    for(auto e: matching_pieces[side]){
        s+= to_string(e) + ", ";
    }
    s+= "}";
    return std::move(s);
}

void PuzzlePiece::insert_matching_piece(int side, int other_piece_id) {
    assert(side>=0);
    assert(side<4);
    assert(other_piece_id != piece_id);
    matching_pieces[side].insert(other_piece_id);
}

PuzzlePiece::PuzzlePiece(int piece_id_) {
    piece_id = piece_id_;
    for(auto e: matching_pieces){
        e = set<int>();
    }
}
PuzzlePiece::PuzzlePiece() {
    piece_id = 0;
    for(auto e: matching_pieces){
        e = set<int>();
    }
}

