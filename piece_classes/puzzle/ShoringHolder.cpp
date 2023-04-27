//
// Created by luca on 4/27/23.
//

#include "ShoringHolder.h"

ShoringHolder::ShoringHolder(): Holder(){
    // copying the image
    rotated_image = get_image().clone();
    // coping the 4 points
    for(int i=0; i<4; i++){
        rotated_points[i] = get_point(i, false);
    }
}

ShoringHolder::ShoringHolder(PieceImage *piece_, int rotation): Holder(piece_, rotation){
    // copying the image
    rotated_image = get_image().clone();
    // coping the 4 points
    for(int i=0; i<4; i++){
        rotated_points[i] = get_point(i, false);
    }
}
