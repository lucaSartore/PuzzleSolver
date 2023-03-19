
#include "Side.h"
#include "Piece.h"
#include <fstream>
#include <utility>

string Piece::origin_path = "";
int Piece::compare_res = 1200;

Piece::Piece(int piece_id, string path) {
    string piece_path = path + string("/") + to_string(piece_id) + string(".jpeg");
    string data_path = path + string("/") + to_string(piece_id) + string(".txt");

    // initialize piece
    piece = imread(piece_path);
    assert(!piece.empty());

    // initialize id
    id = piece_id;

    // get coordinates
    string line;
    ifstream data (data_path);

    assert(data.is_open());

    // fill the points
    for (auto & point : points) {
        std::getline(data, line);
        std::stringstream ss(line);
        char c[6], a;
        int x, y;
        ss >> c >> a >> x >> a >> y >> a;
        point = Point(x, y);
    }

    for(int i=0; i<4; i++){
        sides[i] = Side(piece, this,i,points[i],points[(i+1)%4]);
    }
}

Piece::Piece(int piece_id): Piece(piece_id, origin_path){}

void Piece::set_origin_path(string path) {
    origin_path = std::move(path);
}

Side &Piece::get_side(int index) {
    assert(index >= 0);
    assert(index <4);
    return sides[index];
}

void Piece::set_compare_res(int res) {
    compare_res = res;
}

int Piece::get_compare_res() {
    return compare_res;
}
