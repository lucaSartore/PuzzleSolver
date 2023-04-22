//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "graphic_piece/Side.h"
#include "graphic_piece/PieceShape.h"
#include "graphic_piece/util_piece.h"
#include "logic_piece/PieceConnection.h"
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include "groped_pieces/GroupedPieces.h"

using namespace std;
using namespace std::chrono;

#define NUMBER_OF_PIECES 100
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0


void piece_comparer_thread(PieceConnection pieces_connections[], PieceShape pieces_shapes[], atomic<int> *index);
void calculate_single_thread(bool debug = false);
void calculate_multi_thread(int number_of_threads = 0);


int main(){

    string path = "../../dataset/blue_500pcs/connections";

    PieceConnection pieces[NUMBER_OF_PIECES];

    // load all the pieces
    for(int i=0; i<NUMBER_OF_PIECES; i++){
        pieces[i].became(path,i);
    }

    // empty list of element 1;
    std::list<GroupedPieces<1>> group_lev_1 = {};

    for(auto &piece: pieces){
        for(int orientation=0; orientation<4; orientation++){
            // creating the array with all the possible pieces
            group_lev_1.emplace_front(&piece,orientation);
        }
    }

    // empty list of element 2;
    std::list<GroupedPieces<2>> group_lev_2 = {};

    int c=0;
    // creating list of potential options
    for(auto &first: group_lev_1){
        for(auto &second: group_lev_1){
            for(auto &third: group_lev_1){
                for(auto &fourth: group_lev_1){
                    try{
                        group_lev_2.emplace_front(&first,&second,&third,&fourth);
                    }catch(invalid_argument &e){
                        //cout << "invalid" << endl;
                    }
                    //cout << "shore: " << group_lev_2.front().get_shore().get_shore() << endl;
                    c++;

                }
            }
            cout << (float) c/(400.0*400.0*400.0*400.0) * 100 << "%" << endl;
            cout << c <<" tested combination. " << group_lev_2.size() << " possible found" << endl;
        }
    }


    return 0;

    calculate_single_thread();
    //calculate_multi_thread(2);
}


void piece_comparer_thread(PieceConnection pieces_connections[], PieceShape pieces_shapes[], atomic<int> *index){
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
                    if (compatibility < MINIMUM_COMPATIBILITY_PERCENTAGE) {
                        compatibility = 0;
                    }
                        // add compatibility to the register;
                        pieces_connections[piece_id].insert_matching_piece(piece_side, other_piece_id, other_piece_side, compatibility);
                        pieces_connections[other_piece_id].insert_matching_piece(other_piece_side, piece_id, piece_side, compatibility);
                }
            }
        }
    }
}


// errors!
void calculate_single_thread(bool debug){
    // create array of piece shape
    PieceConnection::set_number_of_pieces(NUMBER_OF_PIECES);
    PieceShape::set_origin_path("../../dataset/blue_500pcs/divided");
    PieceShape pieces_shapes[NUMBER_OF_PIECES];

    // create array of piece logic
    PieceConnection pieces_connections[NUMBER_OF_PIECES];

    // filling both array up with the respective index;
    for(int i=0; i<NUMBER_OF_PIECES;i++){
        pieces_connections[i].became(i);
        pieces_shapes[i] = PieceShape(i);
    }

    auto start = chrono::steady_clock::now();

    // compare all the pieces_shapes one by one, and save the results in the piece logic
    for(int piece_id=0; piece_id<NUMBER_OF_PIECES;piece_id++){
        cout << "done piece: " << piece_id << "/" << NUMBER_OF_PIECES << endl;
        for(int piece_side=0; piece_side<4; piece_side++){

            // if this side is a border i don't need to compare it with others
            //if(pieces_shapes[piece_id].get_side(piece_side).get_kind() == BORDER){
            //    continue;
            //}

            for(int other_piece_id=piece_id+1; other_piece_id<NUMBER_OF_PIECES;other_piece_id++){
                for(int other_piece_side=0; other_piece_side<4; other_piece_side++){

                    // check for compatibility
                    float compatibility = pieces_shapes[piece_id].get_side(piece_side).compare_to(
                            pieces_shapes[other_piece_id].get_side(other_piece_side)
                    );
                    if(compatibility < MINIMUM_COMPATIBILITY_PERCENTAGE){
                        compatibility = 0;
                    }
                    if(debug){
                        pieces_shapes[piece_id].get_side(piece_side).compare_to(
                                pieces_shapes[other_piece_id].get_side(other_piece_side),
                                true
                        );
                    }
                    // add compatibility to the register;
                    pieces_connections[piece_id].insert_matching_piece(piece_side, other_piece_id, other_piece_side, compatibility);
                    pieces_connections[other_piece_id].insert_matching_piece(other_piece_side, piece_id, piece_side, compatibility);
                }
            }

        }

    }
    cout << "out\n";

    auto end = chrono::steady_clock::now();

    cout << "Execution time in seconds [single threaded]: "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec" << endl;

    cout << "out2\n";

    // save the connections information to the disk
    for(auto & i : pieces_connections){
        i.save_as_file("../../dataset/blue_500pcs/connections");
    }

    PieceConnection pc = PieceConnection("../../dataset/blue_500pcs/connections",0);
}



// errors!
void calculate_multi_thread(int number_of_threads){
    PieceConnection::set_number_of_pieces(NUMBER_OF_PIECES);

    // create array of piece shape
    PieceShape::set_origin_path("../../dataset/blue_500pcs/divided");
    PieceShape pieces_shapes[NUMBER_OF_PIECES];

    // create array of piece logic
    PieceConnection pieces_logic[NUMBER_OF_PIECES];

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