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

#define NUMBER_OF_PIECES 500
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0.99

int main(){

    // create array of piece shape
    PieceShape::set_origin_path("../../dataset/blue_500pcs/divided");
    PieceShape pieces_shapes[NUMBER_OF_PIECES];

    // create array of piece logic
    PieceLogic pieces_logic[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces_logic[i] = PieceLogic(i);
        pieces_shapes[i] = PieceShape(i);
    }


    // compare all the pieces_shapes one by one, and save the results in the piece logic
    for(int piece_id=0; piece_id<5;piece_id++){
        cout << "done piece: " << piece_id << "/" << NUMBER_OF_PIECES << endl;
        for(int piece_side=0; piece_side<4; piece_side++){
            for(int other_piece_id=piece_id+1; other_piece_id<500;other_piece_id++){
                for(int other_piece_side=0; other_piece_side<4; other_piece_side++){

                    // check for compatibility
                    float compatibility = pieces_shapes[piece_id].get_side(piece_side).compare_to(
                            pieces_shapes[other_piece_id].get_side(other_piece_side)
                            );
                    if(compatibility > MINIMUM_COMPATIBILITY_PERCENTAGE){
                        // add compatibility to the register;
                        pieces_logic[piece_id].insert_matching_piece(other_piece_id,other_piece_side);
                        pieces_logic[other_piece_id].insert_matching_piece(piece_id,piece_side);
                    }
                }
            }
        }
    }

    // save the connections information to the disk
    for(auto & i : pieces_logic){
        i.save_as_file("../../dataset/blue_500pcs/connections");
    }

    return 0;
}