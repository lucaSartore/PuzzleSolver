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


shared_ptr<Holder> get_best(Puzzle& puzzle, int x, int y){

    auto bests = puzzle.get_best_fits(x,y);
    tuple<float,shared_ptr<Holder>> best_element = bests.front();
    return std::get<1>(best_element);
}

int main(){




    ///*

    //Puzzle puzzle = Puzzle("../../dataset/test_2x3/divided",6);
    Puzzle puzzle = Puzzle("../../dataset/blue_500pcs/divided",500);

    puzzle.array.grow_y();
    puzzle.array.grow_x();
    puzzle.set_min_compatibility(0);


    puzzle.array.set(0,0, get_best(puzzle,0,0));


    for(int i=1; i<5; i++){

        puzzle.array.grow_y();
        puzzle.array.grow_x();

        for(int x=0; x<i; x++){
            puzzle.array.set(x,i, get_best(puzzle,x,i));
        }
        for(int y=0; y<i; y++){
            puzzle.array.set(i,y, get_best(puzzle,i,y));
        }
        puzzle.array.set(i,i, get_best(puzzle,i,i));

    }

    puzzle.array.grow_y();
    puzzle.array.grow_x();

    //for(int x=0; x<2; x++){puzzle.array.set(x,2, get_best(puzzle,x,2));}

    //for(int y=0; y<2; y++){puzzle.array.set(2,y, get_best(puzzle,2,y));}

    Mat img = puzzle.array.get_image();
    Mat resized;
    resize(img,resized,Size(3950,3950));
    imshow("puzzle",resized);
    waitKey(0);
    //*/



    /*

    Puzzle puzzle = Puzzle("../../dataset/test_2x3/divided",6);

    PieceHolder holder_0 = PieceHolder(&puzzle.pieces[4],0);
    PieceHolder holder_1 = PieceHolder(&puzzle.pieces[4],1);
    PieceHolder holder_2 = PieceHolder(&puzzle.pieces[4],2);
    PieceHolder holder_3 = PieceHolder(&puzzle.pieces[4],3);


    for(int index=0; index<4; index++) {
        Mat im_0 = holder_0.get_image();
        Mat im_1 = holder_1.get_image();
        Mat im_2 = holder_2.get_image();
        Mat im_3 = holder_3.get_image();

        Point p_0 = holder_0.get_point(index);
        Point p_1 = holder_1.get_point(index);
        Point p_2 = holder_2.get_point(index);
        Point p_3 = holder_3.get_point(index);

        circle(im_0, p_0, 10, Scalar(127), -1);
        circle(im_1, p_1, 10, Scalar(127), -1);
        circle(im_2, p_2, 10, Scalar(127), -1);
        circle(im_3, p_3, 10, Scalar(127), -1);

        imshow("im_0", im_0);
        imshow("im_1", im_1);
        imshow("im_2", im_2);
        imshow("im_3", im_3);
        waitKey(0);
    }

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

