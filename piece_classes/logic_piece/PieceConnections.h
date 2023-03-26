//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PIECECONNECTIONS_H
#define PIECECLASS_PIECECONNECTIONS_H

#include <iostream>
#include <set>
#include <mutex>

/// this class is used to store the information about the connections pieces have
class PieceConnections {
private:
    int piece_id;
    std::set<std::tuple<int,int>> matching_pieces[4];
    std::string get_side_as_string(int side) const;
    std::mutex mut;
public:
    /// this function save the data inside the PieceConnections in to a file with the name specified in the path
    void save_as_file(std::string path);
    /// zero parameter constructor;
    PieceConnections();
    /// one parameter constructor, it initialize with matching_pieces empty
    explicit PieceConnections(int id);
    /// this is a constructor that create a piece form some data saved by the `save_as_file` path
    explicit PieceConnections(std::string path, int id);
    /// return the ids of the current piece
    int get_piece_id() const;
    /// return a pointer to the set of the pieces close to the specified side
    std::set<std::tuple<int,int>> & get_matching_piece_to_side(int side);
    /// allows ypu to insert a new element in the the piece's compatible pieces
    void insert_matching_piece(int other_piece_id,int side_this_piece, int side_other_piece);
    /// turn the piece into an empty piece with the specified index
    void became(int id);
    /// turn the piece into the saved piece with the specified id and path
    void became(std::string path, int id);
    /// return a string representation of the current piece
    std::string to_string();
};


/// testing function for saving file
void test_file_save();

std::ostream & operator<<(const std::ostream& os,const PieceConnections& piece);

#endif //PIECECLASS_PIECECONNECTIONS_H
