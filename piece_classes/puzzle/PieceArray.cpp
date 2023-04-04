//
// Created by luca on 4/4/23.
//

#include "PieceArray.h"
#include "stdexcept"
using namespace std;


PieceArray::PieceArray() {
    dim_x = 0;
    dim_y = 0;
    pieces = vector<vector<Holder*>>();
}

Holder &PieceArray::get(int x, int y) {
    // check if in range
    if(x<0 || y<0 || x >= dim_x || y >= dim_y){
        throw std::invalid_argument("index out of range");
    }
    return *pieces[x][y];
}

void PieceArray::set(int x, int y, Holder to_be_set) {
    // check if in range
    if(x<0 || y<0 || x > dim_x || y > dim_y){
        throw std::invalid_argument("index out of range");
    }

}


