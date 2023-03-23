//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PIECECONNECTIONS_H
#define PIECECLASS_PIECECONNECTIONS_H

#include <iostream>
#include <set>
#include <mutex>
using namespace std;

/// this class is used to store the information about the connections pieces have
class PieceConnections {
private:
    int piece_id;
    set<int> matching_pieces[4];
    string get_side_as_string(int side) const;
    mutex mut;
public:
    /// this function save the data inside the PieceConnections in to a file with the name specified in the path
    void save_as_file(string path);
    /// zero parameter constructor;
    PieceConnections();
    /// one parameter constructor, it initialize with matching_pieces empty
    explicit PieceConnections(int id);
    /// this is a constructor that create a piece form some data saved by the `save_as_file` path
    explicit PieceConnections(string path, int id);
    /// return the ids of the current piece
    int get_piece_id() const;
    /// return a pointer to the set of the pieces close to the specified side
    set<int> & get_matching_piece_to_side(int side);
    friend ostream & operator<<(ostream& os, PieceConnections& piece);
    /// allows ypu to insert a new element in the the piece's compatible pieces
    void insert_matching_piece(int other_piece_id,int side);
    /// turn the piece into an empty piece with the specified index
    void became(int id);
    /// turn the piece into the saved piece with the specified id and path
    void became(string path, int id);
};

ostream & operator<<(const ostream& os,const PieceConnections& piece);

#endif //PIECECLASS_PIECECONNECTIONS_H
