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
    // path where you can find the images saved
    static string origin_path;
    // the resolution if the comparison mask, if has to be about equal to the PPI of the image
    static int compare_res;
    Side sides[4];
    Point points[4];
    Mat piece;
    int id{};
public:

    // disable copy constructor, since all the pieces will be the same in all the program it make mor sense to use pointers
    Piece(Piece& other) = delete;
    // crete a piece with the givven id from the origin path that has been chosen with `set_origin_path`
    Piece(int piece_id);
    // create a piece with the given id, form the specified path
    Piece(int piece_id, string path);
    // set the origin path that will be used by the one parameter constructor
    static void set_origin_path(string path);
    // set the comparison resolution that has to be used in the side to side cameraperson
    static void set_compare_res(int res);
    // get the desired side
    Side &get_side(int index);
    // get the comparing res
    static int get_compare_res();

};

#endif //PUZZLE_SOLVER_PIECE_H
