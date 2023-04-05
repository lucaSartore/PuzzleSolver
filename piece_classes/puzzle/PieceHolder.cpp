//
// Created by luca on 4/4/23.
//

#include "PieceHolder.h"



char PieceHolder::get_debug_view() const{
    return 'P';
}

float PieceHolder::check_compatibility(Holder *up, Holder *down, Holder *left, Holder *right) {
    // todo
    assert(true);
}

Side *PieceHolder::get_side(Direction direction) {
    return Holder::get_side(direction);
}

PieceHolder::PieceHolder(Piece &&piece_, int orientation_) {
    piece = std::move(piece_);
    orientation = orientation_;
}


