//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_HOLDER_H
#define PIECECLASS_HOLDER_H

#include "../graphic_piece/PieceShape.h"
#include "../graphic_piece/Side.h"

// shorter alias
typedef PieceShape Piece;

#include <opencv2/opencv.hpp>


#include "../groped_pieces/Direction.h"

class Holder {
private:
    cv::Point offset;
    cv::Scalar color;
public:
    /// say if this holder is an unknown holder or not
    virtual bool is_unknown();
    /// say if this holder is an outside holder or not
    virtual bool is_outside();
    /// say if this holder is a piece holder or not
    virtual bool is_a_piece();
    /// return the compatibility of this piece with the others
    virtual float check_compatibility(Holder* up, Holder* down, Holder* left, Holder* right);
    /// return the side on the specified direction,
    /// it returns null ptr if the holder is an unknown holder
    virtual Side* get_side(Direction direction);
    /// set the offset of the piece;
    void set_offset(cv::Point p);
    /// get the offset of the piece;
    cv::Point get_offset();
    /// set the color of the current piece;
    void set_color(cv::Scalar new_color);
    /// return the current color of the piece;
    cv::Scalar get_color();

    Holder();
    virtual ~Holder() = default;
    virtual char get_debug_view() const;
};


#endif //PIECECLASS_HOLDER_H