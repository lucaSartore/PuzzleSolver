//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PIECELOGIC_H
#define PIECECLASS_PIECELOGIC_H

#include <iostream>
#include <set>
#include <mutex>
using namespace std;

class PieceLogic {
private:
    int piece_id;
    set<int> matching_pieces[4];
    string get_side_as_string(int side) const;
    mutex mut;
public:
    /// this function save the data inside the PieceLogic in to a file with the name specified in the path
    void save_as_file(string path);
    /// zero parameter constructor;
    PieceLogic();
    /// one parameter constructor, it initialize with matching_pieces empty
    explicit PieceLogic(int id);
    /// this is a constructor that create a piece form some data saved by the `save_as_file` path
    explicit PieceLogic(string path, int id);
    /// return the ids of the current piece
    int get_piece_id() const;
    /// return a pointer to the set of the pieces close to the specified side
    set<int> & get_matching_piece_to_side(int side);
    friend ostream & operator<<(ostream& os, PieceLogic& piece);
    /// allows ypu to insert a new element in the the piece's compatible pieces
    void insert_matching_piece(int other_piece_id,int side);
    /// turn the piece into an empty piece with the specified index
    void became(int id);
    /// turn the piece into the saved piece with the specified id and path
    void became(string path, int id);
};

ostream & operator<<(const ostream& os,const PieceLogic& piece);

#endif //PIECECLASS_PIECELOGIC_H
