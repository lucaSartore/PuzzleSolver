//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "Side.h"
#include "Piece.h"
#include "util_piece.h"
using namespace std;

int main(){



    Piece::set_origin_path("../../dataset/blue_500pcs/divided");

    Piece p1 = Piece(400);

    for(int i=2; i<500;i++){
        cout << i << "/500" << endl;
        Piece p2 = Piece(i);
        for(int side=0; side < 4; side++){
            float perc = p1.get_side(0).compare_to(p2.get_side(side));
            if(perc > 0.995){
                p1.get_side(0).compare_to(p2.get_side(side), true);
            }
        }
    }

    return 0;
}