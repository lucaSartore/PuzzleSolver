//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PIECELOGIC_H
#define PIECECLASS_PIECELOGIC_H

#include <iostream>
#include <set>

using namespace std;

class PieceLogic {
private:
    int piece_id;
    set<int> matching_pieces[4];
    string get_side_as_string(int side) const;
public:
    /// this function save the data inside the PieceLogic in to a file with the name specified in the path
    void save_as_file(string path);
    /// zero parameter constructor;
    PieceLogic();
    /// one parameter constructor, it initialize with matching_pieces empty
    explicit PieceLogic(int piece_id_);
    /// this is a constructor that create a piece form some data saved by the `save_as_file` path
    explicit PieceLogic(string path, int id);
    /// return the ids of the current piece
    int get_piece_id() const;
    /// return a pointer to the set of the pieces close to the specified side
    set<int> & get_matching_piece_to_side(int side);
    friend ostream & operator<<(ostream& os, PieceLogic& piece);
    /// allows ypu to insert a new element in the the piece's compatible pieces
    void insert_matching_piece(int side, int other_piece_id);
};

ostream & operator<<(const ostream& os,const PieceLogic& piece);

#endif //PIECECLASS_PIECELOGIC_H
