//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PIECEHOLDER_H
#define PIECECLASS_PIECEHOLDER_H

#include "Holder.h"

class PieceHolder: public Holder{
private:
    Piece* piece;
    int orientation;
public:
    PieceHolder(Piece *piece_, int orientation_);
    ~PieceHolder() override = default;
    char get_debug_view() const override;
    /// say if this holder is a piece holder or not
    bool is_a_piece() override;
    /// return the compatibility of this piece with the others
    float check_compatibility(Holder* up, Holder* down, Holder* left, Holder* right) override;
    /// return the side on the specified direction,
    /// it returns null ptr if the holder is an unknown holder
    Side* get_side(Direction direction) override;
    /// return the image held in this holder
    cv::Mat get_image();
    /// return the coordinates (relative to the image obtained with `get_image`) of the offset
    /// of the side at the specified direction
    cv::Point get_side_center(Direction direction);
    /// return the coordinates (relative to the image obtained with `get_image`) of the offset of the piece
    cv::Point get_center();
    /// same as `get_side_center` but keeps track of where the piece is placed in the piece array using the offset
    cv::Point get_side_center_with_offset(Direction direction);
    /// return the coordinates of one of the 4 points of the image, based on the specified index
    cv::Point  get_point(int index);
};


#endif //PIECECLASS_PIECEHOLDER_H
