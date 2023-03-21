//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "grafic_piece/Side.h"
#include "grafic_piece/Piece.h"
#include "grafic_piece/util_piece.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(){

    Piece::set_origin_path("../../dataset/blue_500pcs/divided");

    Piece pieces[500];

    for(int i=0; i<500;i++){
        pieces[i] = Piece(i);
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