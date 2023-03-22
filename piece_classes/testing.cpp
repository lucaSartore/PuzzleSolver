//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "grafic_piece/Side.h"
#include "grafic_piece/PieceShape.h"
#include "grafic_piece/util_piece.h"
#include "logic_piece/PieceLogic.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(){


    PieceLogic piece = PieceLogic(22);
    piece.insert_matching_piece(0,11);
    piece.insert_matching_piece(0,14);
    piece.insert_matching_piece(1,77);
    piece.insert_matching_piece(3,11);
    piece.insert_matching_piece(3,113);
    piece.insert_matching_piece(3,113);

    piece.save_as_file(".");

    PieceLogic piece2 = PieceLogic(".", 22);


    cout << piece << endl;
    cout << piece2 << endl;



    PieceShape::set_origin_path("../../dataset/blue_500pcs/divided");
    PieceShape pieces[500];


    for(int i=0; i<500;i++){
        pieces[i] = PieceShape(i);
    }


    auto start = high_resolution_clock::now();

    for(int i=0; i<500;i++){
        for(int side = 0; side<4; side++){
            float p = pieces[444].get_side(0).compare_to(pieces[i].get_side(side));
            if(p>0.99){
                pieces[444].get_side(0).compare_to(pieces[i].get_side(side), true);
            }
        }
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " milliseconds" << endl;


    return 0;
}