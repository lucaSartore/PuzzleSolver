#include "PieceConnection.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;

int PieceConnection::number_of_pieces = 0;

void PieceConnection::save_as_file(string path) {

    // saving file as txt for readability [debug only]
    string second_path = path;
    second_path += "/" +std::to_string(piece_id) + ".txt";
    FILE* debug_file = fopen(second_path.c_str(),"w");
    fprintf(debug_file,"%s",to_string().c_str());
    fclose(debug_file);

    // calculate path
    path += "/" + std::to_string(piece_id) + ".bin";

    // open as binary
    FILE* file = fopen(path.c_str(),"wb");

    // salve all the values
    for(auto this_side: connections){
        for(int piece = 0; piece < number_of_pieces; piece++){
            for(auto shore: this_side[piece].shores){
                fwrite(&shore,sizeof(float),1,file);
            }
        }
    }

    // close file
    fclose(file);
}

PieceConnection::PieceConnection(string path, int id): PieceConnection() {
    this->became(path,id);
}

void PieceConnection::became(string path, int id) {

    if(number_of_pieces == 0){
        cerr << "WARNING: number of pieces is set to 0. you need to set it to the correct value!" << endl;
    }

    piece_id = id;
    // calculate path
    path += "/" + std::to_string(id) + ".bin";

    // open as binary
    FILE* file;
    if(!(file = fopen(path.c_str(),"rb"))){
        throw runtime_error("file not found");
    }

    // salve all the values
    for(auto this_side: connections){
        for(int piece = 0; piece < number_of_pieces; piece++){
            for(auto &shore: this_side[piece].shores){
                if(!feof(file)){
                    fread(&shore,sizeof(float),1,file);
                }else{
                    throw runtime_error("binary file not compatible");
                }
            }
        }
    }

    // close file
    fclose(file);
}


int PieceConnection::get_piece_id() const{
    return piece_id;
}


ostream & operator<<(ostream& os, PieceConnection& piece){
    os << piece.to_string();
    return os;
}

string PieceConnection::get_side_as_string(int side) const {
    assert(side>=0);
    assert(side<4);
    string s = string("{");
    for(int i=0; i<number_of_pieces; i++){
        auto elem = connections[side][i];
        for(int side_other=0; side_other < 4; side_other++)
        s+= std::to_string(i) + "_" + std::to_string(side_other) + ": " + std::to_string(elem.shores[side_other]) + ", ";
    }
    s+= "}";
    return std::move(s);
}


PieceConnection::PieceConnection(int id): PieceConnection() {
    this->became(id);
}
void PieceConnection::became(int id) {
    mut.lock();

    // insert new piece id
    piece_id = id;

    // reset all the connections
    for(auto c: connections){
        for(int i=0; i<number_of_pieces; i++){
            c[i] = Connection();
        }
    }

    mut.unlock();
}
PieceConnection::PieceConnection() {
    piece_id = 0;
    for(auto &c: connections){
        c = new Connection[number_of_pieces];
    }
}

string PieceConnection::to_string() {
    string s;

    s += "piece_id: " + std::to_string(piece_id) +
            "\nside 0: " + get_side_as_string(0) +
            "\nside 1: " + get_side_as_string(1) +
            "\nside 2: " + get_side_as_string(2) +
            "\nside 3: " + get_side_as_string(3);

    return s;
}

void PieceConnection::set_number_of_pieces(int new_val) {
    number_of_pieces = new_val;
}

int PieceConnection::get_number_of_pieces() {
    return number_of_pieces;
}

void PieceConnection::insert_matching_piece(int side_this_piece,int other_piece_id,int other_piece_side, float shore) {
    assert(side_this_piece>=0);
    assert(side_this_piece<4);
    assert(other_piece_side>=0);
    assert(other_piece_side<4);
    assert(other_piece_id>=0);
    assert(other_piece_id<number_of_pieces);
    mut.lock();
    connections[side_this_piece][other_piece_id].shores[other_piece_side] = shore;
    mut.unlock();
}

PieceConnection::~PieceConnection() {
    for(auto p: connections){
        delete[] p;
    }
}

float PieceConnection::compare(int this_piece_side, int other_piece_id, int other_piece_side) {
    return connections[this_piece_side][other_piece_id].shores[other_piece_side];
}

/// testing function for saving file
void test_file_save(){
    PieceConnection p1(10);


    p1.save_as_file(".");

    PieceConnection p2(".", 10);

    p2.save_as_file("..");

    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;

    assert(p1.to_string() == p2.to_string());

}



Connection::Connection() {
    for(auto &d: shores){
        d = 0;
    }
}
