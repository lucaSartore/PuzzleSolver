//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "Side.h"
#include "Piece.h"
#include "util_piece.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(){

    Piece::set_origin_path("../../dataset/blue_500pcs/divided");

    Piece pieces[500];

    for(int i=1; i<500;i++){
        pieces[i-1] = Piece(i);
    }


    auto start = high_resolution_clock::now();

    for(int i=0; i<499;i++){
        for(int side = 0; side<4; side++){
            float p = pieces[3].get_side(0).compare_to(pieces[i].get_side(side));
            //if(p>0.995){
            //    pieces[3].get_side(0).compare_to(pieces[i].get_side(side), true);
            //}
        }
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;


    return 0;
}