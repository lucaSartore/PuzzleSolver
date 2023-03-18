//
// Created by luca on 3/18/23.
//

#ifndef PUZZLE_SOLVER_PIECE_H
#define PUZZLE_SOLVER_PIECE_H

#include "Side.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Piece {
private:
    static string origin_path;
    Side sides[4];
    Mat piece;
    int id;
public:
    // crete a piece with the givven id from the origin path that has been chosen with `set_origin_path`
    Piece(int piece_id);
    // create a piece with the given id, form the specified path
    Piece(int piece_id, string path);
    // set the origin path that will be used by the one parameter constructor
    void set_origin_path(string path);
    // get the desired side
    Side &get_side(int index);
};

#endif //PUZZLE_SOLVER_PIECE_H
