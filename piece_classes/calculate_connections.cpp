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
#include "puzzle/Puzzle.h"

using namespace std;
using namespace std::chrono;
using namespace cv;

#define NUMBER_OF_PIECES 500
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0.5


int main(){


    Puzzle puzzle = Puzzle("../../dataset/blue_500pcs/divided",10);

    /*
    // test for the puzzle array visualization
    Piece::set_origin_path("../../dataset/blue_500pcs/divided");
    Piece p = Piece(0);
    PieceArray pa = PieceArray();

    pa.set(0,0,shared_ptr<Holder>(new PieceHolder(&p,0)));


    for(int i=1; i<10; i++){

        for(int x=0; x<i; x++){
            pa.set(x,i,shared_ptr<Holder>(new PieceHolder(&p,0)));
        }
        for(int y=0; y<i; y++){
            pa.set(i,y,shared_ptr<Holder>(new PieceHolder(&p,0)));

        }
        pa.set(i,i,shared_ptr<Holder>(new PieceHolder(&p,0)));

    }
    Mat image = pa.get_image();
    Mat resized;
    resize(image,resized,Size(950,950));
    imshow("puzzle",resized);
    waitKey(0);

    pa.remove(0,9);
    pa.remove(0,8);
    pa.remove(9,7);

    image = pa.get_image();
    resize(image,resized,Size(950,950));
    imshow("puzzle",resized);
    waitKey(0);
    */

    /*
    // test for the orientation and get point finctions
    int orientation = 2;
    Direction direction = RIGHT;

    Piece::set_origin_path("../../dataset/blue_500pcs/divided");
    Piece p = Piece(0);


    PieceArray pa = PieceArray();

    pa.set(0,0,shared_ptr<Holder>(new PieceHolder(&p,orientation)));

    Holder* h = pa.get(0,0);

    if(h->is_a_piece()){
        auto* ph = dynamic_cast<PieceHolder*>(h);
        Mat image = ph->get_image().clone();
        circle(image,ph->get_side_center(direction),10,Scalar(125),-1);
        Mat side = ph->get_side(direction)->get_side_mask();
        Mat resized;
        resize(image,resized,image.size()*2/3);
        imshow("Piece", resized);
        resize(side,resized,side.size()/2);
        imshow("Side", resized);
        waitKey(0);
    }*/


    /*
    // see the array functions with the debug print
    PieceArray array = PieceArray();

    cout << array << endl;

    array.grow_x();array.grow_x();array.grow_x();

    cout << array << endl;

    array.grow_y();

    cout << array << endl;

    Piece::set_origin_path("../../dataset/blue_500pcs/divided");
    Piece p = Piece(0);
    auto h = shared_ptr<Holder>(new PieceHolder(&p,0));
    array.set(0,0,h);

    cout << array << endl;

    array.grow_x();
    array.grow_y();

    cout << array << endl;

    array.un_grow_x();
    cout << array << endl;

    array.un_grow_y();
    cout << array << endl;

    array.remove(0,0);
    cout << array << endl;

    */
    /*
    // array creation
    Piece::set_origin_path("../../dataset/blue_500pcs/divided");
    Piece pieces[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces[i] = Piece(i);
    }*/
}

