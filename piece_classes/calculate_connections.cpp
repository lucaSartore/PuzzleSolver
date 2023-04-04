//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "piece/Side.h"
#include "piece/Piece.h"
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>

using namespace std;
using namespace std::chrono;

#define NUMBER_OF_PIECES 500
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0.5



int main(){
    Piece::set_origin_path("../../dataset/blue_500pcs/divided");
    Piece pieces[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces[i] = Piece(i);
    }
}