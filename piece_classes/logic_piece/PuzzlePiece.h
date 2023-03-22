//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PUZZLEPIECE_H
#define PIECECLASS_PUZZLEPIECE_H

#include <iostream>
#include <set>

using namespace std;

class PuzzlePiece {
private:
    int piece_id;
    set<int> matching_pieces[4];
    string get_side_as_string(int side) const;
public:
    /// this function save the data inside the PuzzlePiece in to a file with the name specified in the path
    void save_as_file(string path);
    /// one parameter constructor, it initialize with matching_pieces empty
    explicit PuzzlePiece(int piece_id_);
    /// this is a constructor that create a piece form some data saved by the `save_as_file` path
    explicit PuzzlePiece(string path, int id);
    /// return the ids of the current piece
    int get_piece_id() const;
    /// return a pointer to the set of the pieces close to the specified side
    set<int> & get_matching_piece_to_side(int side);
    friend ostream & operator<<(ostream& os, PuzzlePiece& piece);
    /// allows ypu to insert a new element in the the piece's compatible pieces
    void insert_matching_piece(int side, int other_piece_id);
};

ostream & operator<<(const ostream& os,const PuzzlePiece& piece);

#endif //PIECECLASS_PUZZLEPIECE_H
