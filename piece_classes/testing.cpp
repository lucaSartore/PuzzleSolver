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

    Piece piece = Piece(1);

    Point p1(0,0),p2(-1,-1);

    cout << find_angle_of_2_points(p1,p2) * 180 / 3.1415926535898 << endl;


    return 0;
}