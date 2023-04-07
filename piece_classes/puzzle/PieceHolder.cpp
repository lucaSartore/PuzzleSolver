//
// Created by luca on 4/4/23.
//

#include "PieceHolder.h"



char PieceHolder::get_debug_view() const{
    return 'P';
}

float PieceHolder::check_compatibility(Holder *up, Holder *down, Holder *left, Holder *right){

    Direction directions[] = {UP,DOWN,RIGHT,LEFT};
    Holder* holders[] = {up,down,right,left};

    for(int i=0; i<4; i++){

        // the direction i'm comparing to
        Direction direction = directions[i];
        // the holder i'm comparing to
        Holder* to_compare = holders[i];

        // if the piece is unknown i skip
        if(to_compare->is_unknown()){
            continue;
        }

        // the side of this piece i am comparing
        Side* my_side = this->get_side(direction);
        // the side of the other piece i am comparing
        Side* his_side = to_compare->get_side(-direction);
    }



    // todo
    assert(true);
}

Side *PieceHolder::get_side(Direction direction) {
    return Holder::get_side(direction);
}

PieceHolder::PieceHolder(Piece *piece_, int orientation_) {
    piece = piece_;
    orientation = orientation_;
}

