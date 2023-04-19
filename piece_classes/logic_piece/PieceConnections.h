//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PIECECONNECTIONS_H
#define PIECECLASS_PIECECONNECTIONS_H

#include <iostream>
#include <mutex>


struct Connection{
    int piece_id;
    int side;
    float shore;
    Connection(int piece_, int side_);
};

/// this class is used to store the information about the connections pieces have
class PieceConnections {
private:
    // the total number of pieces
    static int number_of_pieces;
    // the id of this piece
    int piece_id;
    // connections to the 4 sides
    Connection* connections[4];
    std::string get_side_as_string(int side) const;
    std::mutex mut;
public:
    /// allows to set the number of pieces in the simulation, this is important
    /// to allocate the correct amount of memory when creating the array
    static void set_number_of_pieces(int new_val);
    /// allows to get the number of pieces in the simulation, this is important
    /// to allocate the correct amount of memory when creating the array
    static int get_number_of_pieces(int new_val);
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
    /// return a pointer to the array of the pieces ans the shores to a specified side
    Connection &get_matching_piece_to_side(int side);
    /// allows ypu to insert a new element in the the piece's compatible pieces
    void insert_matching_piece(int side_this_piece, Connection &new_connection);
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
