//
// Created by luca on 3/21/23.
//

#ifndef PIECECLASS_PIECECONNECTION_H
#define PIECECLASS_PIECECONNECTION_H

#include <iostream>
#include <mutex>


class Connection{
public:
    float shores[4];
    Connection();
};

/// this class is used to store the information about the connections piece have
class PieceConnection {
private:
    // the total number of piece
    static int number_of_pieces;
    // the id of this piece
    int piece_id;
    // connections to the 4 sides
    Connection* connections[4];
    std::string get_side_as_string(int side) const;
    std::mutex mut;
public:
    /// allows to set the number of piece in the simulation, this is important
    /// to allocate the correct amount of memory when creating the array
    static void set_number_of_pieces(int new_val);
    /// allows to get the number of piece in the simulation, this is important
    /// to allocate the correct amount of memory when creating the array
    static int get_number_of_pieces();
    /// this function save the data inside the PieceConnection in to a file with the name specified in the path
    void save_as_file(std::string path);
    /// zero parameter constructor;
    PieceConnection();
    /// one parameter constructor, it initialize with matching_pieces empty
    explicit PieceConnection(int id);
    /// this is a constructor that create a piece form some data saved by the `save_as_file` path
    explicit PieceConnection(std::string path, int id);
    /// distructor
    ~PieceConnection();
    /// return the ids of the current piece
    int get_piece_id() const;
    /// allows ypu to insert a new element in the the piece's compatible piece
    void insert_matching_piece(int side_this_piece,int other_piece_id,int other_piece_side, float shore);
    /// turn the piece into an empty piece with the specified index
    void became(int id);
    /// turn the piece into the saved piece with the specified id and path
    void became(std::string path, int id);
    /// return a string representation of the current piece
    std::string to_string();
    /// let you compare this piece with an other piece.
    float compare(int this_piece_side, int other_piece_id, int other_piece_side);
};


/// testing function for saving file
void test_file_save();

std::ostream & operator<<(const std::ostream& os,const PieceConnection& piece);

#endif //PIECECLASS_PIECECONNECTION_H
