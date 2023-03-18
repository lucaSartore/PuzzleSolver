//
// Created by luca on 3/18/23.
//

#include "Piece.h"
#include <fstream>
#include <utility>

string Piece::origin_path = "";

Piece::Piece(int piece_id, string path) {
    string piece_path = path + string("/") + to_string(piece_id) + string(".jpeg");
    string data_path = path + string("/") + to_string(piece_id) + string(".txt");

    // initialize piece
    piece = imread(piece_path);
    assert(!piece.empty());

    imshow("a",piece);
    waitKey(0);

    // initialize id
    id = piece_id;

    // get coordinates
    string line;
    ifstream data (data_path);

    assert(data.is_open());
    while ( getline (data, line) )
    {
        cout << line << '\n';
    }


}

void Piece::set_origin_path(string path) {
    origin_path = std::move(path);
}

Side &Piece::get_side(int index) {
    assert(index >= 0);
    assert(index <4);
    return sides[index];
}
