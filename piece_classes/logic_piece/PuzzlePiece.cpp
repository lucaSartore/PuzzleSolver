#include "PuzzlePiece.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <assert.h>

void PuzzlePiece::save_as_file(string path) {
    std::ofstream outfile;
    outfile.open(path, std::ios::out | std::ios::binary);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file " << path << " for writing\n";
        return;
    }
    outfile.write(reinterpret_cast<const char*>(&piece_id), sizeof(piece_id));
    for (const auto& matches : matching_pieces) {
        int size = static_cast<int>(matches.size());
        outfile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        std::copy(matches.begin(), matches.end(), std::ostream_iterator<int>(outfile, " "));
    }
    outfile.close();
}

PuzzlePiece::PuzzlePiece(string path) {
    std::ifstream infile;
    infile.open(path, std::ios::in | std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file " << path << " for reading\n";
        return;
    }
    infile.read(reinterpret_cast<char*>(&piece_id), sizeof(piece_id));
    for (auto& matches : matching_pieces) {
        int size;
        infile.read(reinterpret_cast<char*>(&size), sizeof(size));
        std::string line;
        std::getline(infile, line);
        std::istringstream iss(line);
        for (int i = 0; i < size; ++i) {
            int val;
            if (iss >> val) {
                matches.insert(val);
            } else {
                std::cerr << "Failed to parse match " << i << " for piece " << piece_id << "\n";
            }
        }
    }
    infile.close();
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
