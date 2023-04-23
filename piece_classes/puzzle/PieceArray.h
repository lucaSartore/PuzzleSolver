//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PIECEARRAY_H
#define PIECECLASS_PIECEARRAY_H

#include "Holder.h"
#include "vector"
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>


class PieceArray {
private:
    cv::Mat image;
    int dim_x;
    int dim_y;
    std::vector<std::vector<Holder>> pieces;
    /// check if the index is or not inside the matrix, if not it throws an error
    void check_indexes(int x, int y) const;
    /// build the image of the puzzle
    void build_image();
    /// check if the image needs to be grown, and eventually do so
    void check_and_expand_image();
    /// this function takes to image and 2 points, it then paste the source inamge on top of the destination image
    /// in a way that the 2 reference point will be one on top of the other
    /// by default the function will paste it on top, but there is the option of doing a bitwise or by using the flag
    static void paste_on_top(const cv::Mat& source, cv::Mat& destination, cv::Point2i pointSource, cv::Point2i pointDestination, bool bitwise_or = false);
    /// insert the piece at the specified coordinates in the image
    void insert_into_image(int x, int y);
    /// get a random (but very saturate color)
    static cv::Scalar get_random_color();
    /// reset the image, and rebuild it
    void reset_image();
public:
    /// returns the x dimension of the 2d array
    int get_dim_x() const;
    /// returns the y dimension of the 2d array
    int get_dim_y() const;
    /// create an empty piece array
    PieceArray();
    /// move constructor
    PieceArray(PieceArray && other);
    /// return a piece_holder in one of the many position
    /// if the coordinates are out of bound it returns null ptd
    Holder* get(int x, int y);
    /// set a piece_holder in one position
    void set(int x, int y, Holder && to_be_set,bool update_graphic = true);
    /// grow the array by 1 in the X dimension
    void grow_x();
    /// grow the array by 1 in the Y dimension
    void grow_y();
    /// un grow the array by 1 in the X dimension
    void un_grow_x();
    /// un grow the array by 1 in the Y dimension
    void un_grow_y();
    /// return the image of the matrix of puzzle pieces
    cv::Mat get_image() const;
    /// move equal operator
    PieceArray& operator=(PieceArray&& other);
};

std::ostream& operator<<(std::ostream& os, const PieceArray& pa);

#endif //PIECECLASS_PIECEARRAY_H
