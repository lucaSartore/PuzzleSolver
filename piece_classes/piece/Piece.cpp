
#include "Side.h"
#include "Piece.h"
#include <fstream>
#include <utility>

using namespace std;
using namespace cv;

string Piece::origin_path = "";

Piece::Piece(int piece_id, string path) {
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

Piece::Piece(int piece_id): Piece(piece_id, origin_path){}

void Piece::set_origin_path(string path) {
    origin_path = std::move(path);
}

Side &Piece::get_side(int index) {
    assert(index >= 0);
    assert(index <4);
    return sides[index];
}

void Piece::show_debug(int side_to_highlight) {
    Mat to_show;
    cvtColor(piece,to_show,COLOR_GRAY2BGR);
    for(int i=0; i<4; i++){
        Scalar color;
        if(side_to_highlight == i){
            color = Scalar(0,255,0);
        }else{
            color = Scalar(0,0,255);
        }
        line(to_show,points[i],points[(i+1)%4],color,4);
    }
    Mat resized;
    resize(to_show,resized,to_show.size()/2);
    imshow("Debug", resized);
    waitKey(0);
}


