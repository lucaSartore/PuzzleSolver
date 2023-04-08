//
// Created by luca on 3/18/23.
//
#ifndef PUZZLE_SOLVER_CORNER_H
#define PUZZLE_SOLVER_CORNER_H

#include <opencv2/opencv.hpp>


class Piece;

typedef enum{BORDER,MALE,FEMALE}SideKind;

class Side {
private:
    // the resolution if the comparison mask, if has to be about equal to the PPI of the image
    static int compare_res;
    // a image containing the shape of the border ready compared it with others
    cv::Mat border_shape;
    // pointer to the original piece
    Piece* piece;
    // an integer referring on witch side the of the original piece this border appear to
    int piece_side;
    // an enum that defines with kind of border this one si
    SideKind kind;
    // keeps the length of the piece, this is useful because piece with different lenghts are not comperable
    int side_lenght;
public:

    // disable copy constructor, since all the pieces will be the same in all the program it make mor sense to use pointers
    Side(Side& other) = delete;

    Side();
    // constructor
    Side(cv::Mat& piece_mask, Piece* piece_, int piece_side_, cv::Point p1, cv::Point p2, cv::Point center);

    // a comparing function that returns a percentage thad indicate how match the 2 pieces are comparable
    float compare_to(const Side &other, bool debug = false)const ;

    // get the kind of the border
    SideKind get_kind();

    // return the piece this border appertains to
    Piece &get_piece();

    // set the comparison resolution that has to be used in the side to side cameraperson
    static void set_compare_res(int res);
    // get the desired side

    // get the comparing res
    static int get_compare_res();

    /// get the mask of the current side;
    cv::Mat get_side_mask(){
        return border_shape.clone();
    }

};


#endif //PUZZLE_SOLVER_CORNER_H
