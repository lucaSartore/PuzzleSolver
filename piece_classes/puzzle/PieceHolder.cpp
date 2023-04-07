//
// Created by luca on 4/4/23.
//

#include "PieceHolder.h"



char PieceHolder::get_debug_view() const{
    return 'P';
}


// the function return the average of the compatibility of the know pieces
// it return 0 if the possibility of a compatibility is 0, for example if we have a male-male cameraperson
float PieceHolder::check_compatibility(Holder *up, Holder *down, Holder *left, Holder *right){

    Direction directions[] = {UP,DOWN,RIGHT,LEFT};
    Holder* holders[] = {up,down,right,left};


    int count=0;
    float average=0;

    for(int i=0; i<4; i++){

        float compatibility;

        // the direction i'm comparing to
        Direction direction = directions[i];
        // the holder i'm comparing to
        Holder* to_compare = holders[i];

        // if the piece is unknown i skip it
        if(to_compare->is_unknown()){
            continue;
        }

        // the side of this piece i am comparing
        Side* my_side = this->get_side(direction);
        // the side of the other piece i am comparing
        Side* his_side = to_compare->get_side(-direction);

        // if i'm comparing with an outside piece, the piece is compatible only if
        // he is a side
        if(to_compare->is_outside()){
            if(my_side->get_kind() != SideKind::BORDER){
                return  0.0;
            }
        }else{
            // otherwise the side is normal, and so i do to the normal compare between pieces
            compatibility = my_side->compare_to(*his_side);
            if(compatibility == 0.0){
                return 0.0;
            }
            average += compatibility;
            count++;
        }
    }

    // return the average of the
    return  average/((float)count);
}

Side *PieceHolder::get_side(Direction direction) {
    int offset;
    switch (direction) {
        case UP:
            offset=0;
            break;
        case RIGHT:
            offset=1;
            break;
        case DOWN:
            offset=2;
            break;
        case LEFT:
            offset=3;
            break;
        default:
            throw std::runtime_error("unknown direction");
    }
    return &piece->get_side((orientation+offset)%4);
}

PieceHolder::PieceHolder(Piece *piece_, int orientation_) {
    assert(piece!= nullptr);
    assert(orientation_>=0 && orientation_<4);

    piece = piece_;
    orientation = orientation_;
}

