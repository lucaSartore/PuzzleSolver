
#include "Side.h"
#include "PieceShape.h"
#include <fstream>
#include <utility>

string PieceShape::origin_path = "";

PieceShape::PieceShape(int piece_id, string path) {
    string piece_path = path + string("/") + to_string(piece_id) + string(".jpeg");
    string data_path = path + string("/") + to_string(piece_id) + string(".txt");

    // initialize piece
    piece = imread(piece_path,IMREAD_GRAYSCALE);
    assert(!piece.empty());

    // initialize id
    id = piece_id;

    // get coordinates
    string line;
    ifstream data (data_path);

    assert(data.is_open());

    // read the points
    for (auto & point : points) {
        std::getline(data, line);
        std::stringstream ss(line);
        char c[6], a;
        int x, y;
        ss >> c >> a >> x >> a >> y >> a;
        point = Point(x, y);
    }

    //calculate center of the piece
    Point center = (points[0]+points[1]+points[2]+points[3])/4;

    // create sides
    for(int i=0; i<4; i++){
        sides[i] = Side(piece, this,i,points[i],points[(i+1)%4],center);
    }

}

PieceShape::PieceShape(int piece_id): PieceShape(piece_id, origin_path){}

void PieceShape::set_origin_path(string path) {
    origin_path = std::move(path);
}

Side &PieceShape::get_side(int index) {
    assert(index >= 0);
    assert(index <4);
    return sides[index];
}

PieceShape::PieceShape() {}

