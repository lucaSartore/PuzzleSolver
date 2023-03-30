//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "graphic_piece/Side.h"
#include "graphic_piece/PieceShape.h"
#include "graphic_piece/util_piece.h"
#include "logic_piece/PieceConnections.h"
#include <chrono>
#include <memory>
#include "full_puzzle_graph/PuzzleGraph.h"
#include <atomic>
#include <thread>

using namespace std;
using namespace std::chrono;

#define NUMBER_OF_PIECES 500
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0.98


void piece_comparer_thread(PieceConnections pieces_logic[], PieceShape pieces_shapes[], atomic<int> *index);
void calculate_single_thread(bool debug = false);
void calculate_multi_thread(int number_of_threads = 0);
void simplify_graph();

int main(){
    simplify_graph();
    //calculate_single_thread(true);
    //calculate_multi_thread(2);
}


void piece_comparer_thread(PieceConnections pieces_logic[], PieceShape pieces_shapes[], atomic<int> *index){
    while (true) {
        int piece_id = (*index)++;

        cout << "processing piece: " << piece_id << "/" << NUMBER_OF_PIECES << endl;

        if (piece_id >= NUMBER_OF_PIECES - 1) {
            return;
        }

        for (int piece_side = 0; piece_side < 4; piece_side++) {
            for (int other_piece_id = piece_id + 1; other_piece_id < NUMBER_OF_PIECES; other_piece_id++) {
                for (int other_piece_side = 0; other_piece_side < 4; other_piece_side++) {

                    // check for compatibility
                    float compatibility = pieces_shapes[piece_id].get_side(piece_side).compare_to(
                            pieces_shapes[other_piece_id].get_side(other_piece_side)
                    );
                    if (compatibility > MINIMUM_COMPATIBILITY_PERCENTAGE) {
                        // add compatibility to the register;
                        pieces_logic[piece_id].insert_matching_piece(other_piece_id,piece_side, other_piece_side);
                        pieces_logic[other_piece_id].insert_matching_piece(piece_id,other_piece_side, piece_side);
                    }
                }
            }
        }
    }
}


void simplify_graph(){
    PuzzleGraph pg = PuzzleGraph("../../dataset/tests/connections");
    //PuzzleGraph pg = PuzzleGraph("../../dataset/blue_500pcs/connections");

    int excluded_pieces =1;
    while (excluded_pieces) {
        pg.calculate_distances();
        excluded_pieces = pg.exclude_some_connections();
        pg.reset_distances();
    }


    for(int i=0; i<pg.number_of_pieces; i++){
        cout << pg.pieces[i].to_string() << endl;
    }

}


void calculate_single_thread(bool debug){
    // create array of piece shape
    PieceShape::set_origin_path("../../dataset/blue_500pcs/divided");
    PieceShape pieces_shapes[NUMBER_OF_PIECES];

    // create array of piece logic
    PieceConnections pieces_logic[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces_logic[i].became(i);
        pieces_shapes[i] = PieceShape(i);
    }

    auto start = chrono::steady_clock::now();

    // compare all the pieces_shapes one by one, and save the results in the piece logic
    for(int piece_id=0; piece_id<NUMBER_OF_PIECES;piece_id++){
        cout << "done piece: " << piece_id << "/" << NUMBER_OF_PIECES << endl;
        for(int piece_side=0; piece_side<4; piece_side++){
            for(int other_piece_id=piece_id+1; other_piece_id<500;other_piece_id++){
                for(int other_piece_side=0; other_piece_side<4; other_piece_side++){

                    // check for compatibility
                    float compatibility = pieces_shapes[piece_id].get_side(piece_side).compare_to(
                            pieces_shapes[other_piece_id].get_side(other_piece_side)
                    );
                    if(compatibility > MINIMUM_COMPATIBILITY_PERCENTAGE){
                        if(debug){
                            pieces_shapes[piece_id].get_side(piece_side).compare_to(
                                    pieces_shapes[other_piece_id].get_side(other_piece_side),
                                    true
                            );
                        }
                        // add compatibility to the register;
                        pieces_logic[piece_id].insert_matching_piece(other_piece_id,piece_side,other_piece_side);
                        pieces_logic[other_piece_id].insert_matching_piece(piece_id,other_piece_side,piece_side);
                    }
                }
            }
        }
    }

    auto end = chrono::steady_clock::now();

    cout << "Execution time in seconds [single threaded]: "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec";

    // save the connections information to the disk
    for(auto & i : pieces_logic){
        i.save_as_file("../../dataset/blue_500pcs/connections");
    }
}


void calculate_multi_thread(int number_of_threads){
    // create array of piece shape
    PieceShape::set_origin_path("../../dataset/blue_500pcs/divided");
    PieceShape pieces_shapes[NUMBER_OF_PIECES];

    // create array of piece logic
    PieceConnections pieces_logic[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces_logic[i].became(i);
        pieces_shapes[i] = PieceShape(i);
    }

    // create a shared index
    atomic<int> index;

    unsigned  int processor_count;
    if(number_of_threads == 0){
        // find how many cores are available
        processor_count = std::thread::hardware_concurrency();
    }else{
        processor_count = number_of_threads;
    }



    // make share the number is detected correctly
    assert(processor_count != 0);

    auto *threads = new thread[processor_count];

    auto start = chrono::steady_clock::now();

    // launch all threads
    for(int i=0; i<processor_count; i++){
        threads[i] = thread(piece_comparer_thread,pieces_logic,pieces_shapes, &index);
    }

    // showing debug
    while (index < NUMBER_OF_PIECES){
        cout << "processing: " << index << "/" << NUMBER_OF_PIECES << endl;
        std::this_thread::sleep_for (std::chrono::milliseconds (100));
    }

    // join all threads
    for(int i=0; i<processor_count; i++){
        threads[i].join();
    }

    auto end = chrono::steady_clock::now();

    cout << "Execution time in seconds [multi threaded]: "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec";

    // free the memory allocated
    delete[] threads;

    // save the connections information to the disk
    for(auto & i : pieces_logic){
        i.save_as_file("../../dataset/blue_500pcs/connections");
    }
}