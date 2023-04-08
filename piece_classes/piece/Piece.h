//
// Created by luca on 3/18/23.
//

#ifndef PUZZLE_SOLVER_PIECE_H
#define PUZZLE_SOLVER_PIECE_H

#include "Side.h"
#include <iostream>
#include <opencv2/opencv.hpp>


class Piece {
private:
    // path where you can find the images saved
    static std::string origin_path;
    Side sides[4];
    cv::Point points[4];
    cv::Mat piece;
    int id;
public:
    /// default constructor
    Piece() = default;
    ///( disable copy constructor, since all the pieces will be the same in all the program it make mor sense to use pointers
    Piece(Piece& other) = delete;
    /// crete a piece with the given id from the origin path that has been chosen with `set_origin_path`
    Piece(int piece_id);
    /// create a piece with the given id, form the specified path
    Piece(int piece_id, std::string path);
    /// set the origin path that will be used by the one parameter constructor
    static void set_origin_path(std::string path);
    /// get the desired side
    Side &get_side(int index);
    /// show the image with the corner to dbug
    void show_debug(int side_to_highlight = -1);
    /// return the original image
    cv::Mat get_image();
    /// return one of the 4 point, based on the index
    cv::Point get_point(int index);
};

#endif //PUZZLE_SOLVER_PIECE_H
