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
#include "puzzle/Holder.h"
#include "puzzle/PieceArray.h"
#include "puzzle/PieceHolder.h"
#include "puzzle/OutsideHolder.h"

using namespace std;
using namespace std::chrono;

#define NUMBER_OF_PIECES 500
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0.5


int main(){

    Direction directions[] = {UP,DOWN,RIGHT,LEFT};
    for(auto direction: directions){
        cout << "-" << direction << "=" << (-direction) << endl;
    }

    return 0;
    PieceArray array = PieceArray();

    cout << array << endl;

    array.grow_x();array.grow_x();array.grow_x();

    cout << array << endl;

    array.grow_y();

    cout << array << endl;

    auto h = shared_ptr<Holder>(new OutsideHolder());
    array.set(3,1,h);

    cout << array << endl;

    array.grow_x();
    array.grow_y();

    cout << array << endl;

    array.un_grow_x();
    cout << array << endl;

    array.un_grow_y();
    cout << array << endl;

    array.remove(3,1);
    cout << array << endl;


    /*
    Piece::set_origin_path("../../dataset/blue_500pcs/divided");
    Piece pieces[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces[i] = Piece(i);
    }*/
}