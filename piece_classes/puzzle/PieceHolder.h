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
    /// return the compatibility of this piece with the others
    float check_compatibility(Holder* up, Holder* down, Holder* left, Holder* right) override;
    /// return the side on the specified direction,
    /// it returns null ptr if the holder is an unknown holder
    Side* get_side(Direction direction) override;
};


#endif //PIECECLASS_PIECEHOLDER_H
