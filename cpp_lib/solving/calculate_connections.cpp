//
// Created by luca on 3/18/23.
//

#include <iostream>
#include "graphic_piece/Side.h"
#include "graphic_piece/PieceShape.h"
#include "logic_piece/PieceConnection.h"
#include <chrono>
#include <atomic>
#include <thread>

using namespace std;
using namespace std::chrono;

#include "constants.h"
#include "puzzle_preview/PreviewManager.h"

#include "calculate_connections.h"


void piece_comparer_thread(PieceConnection *pieces_connections, PieceShape *pieces_shapes,unsigned int number_of_pieces,  atomic<int> *index);
void calculate_single_thread(const string &input_path, const string &output_path,const unsigned int number_of_pieces, const bool debug);
void calculate_multi_thread(const string &input_path, const string &output_path,unsigned int number_of_cores,const unsigned int number_of_pieces);


void calculate_all_connections(std::string input_path, std::string output_path,unsigned int number_of_pieces, unsigned int number_of_cores, bool debug){
    if(number_of_cores == 1){
        calculate_single_thread(input_path,output_path,number_of_pieces,debug);
    }else{
        // impossible to use debug if number_of_cores is not 1!
        assert(!debug);
        calculate_multi_thread(input_path,output_path,number_of_cores,number_of_pieces);
    }
}

void piece_comparer_thread(PieceConnection *pieces_connections, PieceShape *pieces_shapes,unsigned int number_of_pieces, atomic<int> *index){
    while (true) {
        int piece_id = std::atomic_fetch_add(index,1);

        //cout << "processing piece: " << piece_id << "/" << number_of_pieces << endl;

        if (piece_id >= number_of_pieces - 1) {
            return;
        }

        for (int piece_side = 0; piece_side < 4; piece_side++) {
            for (int other_piece_id = piece_id + 1; other_piece_id < number_of_pieces; other_piece_id++) {
                for (int other_piece_side = 0; other_piece_side < 4; other_piece_side++) {

                    // check for compatibility
                    float compatibility = pieces_shapes[piece_id].get_side(piece_side).compare_to(
                            pieces_shapes[other_piece_id].get_side(other_piece_side)
                    );
                    if (compatibility < MIN_COMPAT_CC) {
                        compatibility = 0;
                    }


                    //Mat preview = pieces_shapes[piece_id].get_side(piece_side).get_compare_image(
                    //        pieces_shapes[other_piece_id].get_side(other_piece_side)
                    //        );
                    //imwrite("comparison.png",preview);

                    if(PreviewManager::is_preview_enabled()){
                        Mat img = pieces_shapes[piece_id].get_side(piece_side).get_compare_image(
                                pieces_shapes[other_piece_id].get_side(other_piece_side)
                        );
                        PreviewManager::output_preview_image(img);
                    }

                    // add compatibility to the register;
                    pieces_connections[piece_id].insert_matching_piece(piece_side, other_piece_id, other_piece_side, compatibility);
                    pieces_connections[other_piece_id].insert_matching_piece(other_piece_side, piece_id, piece_side, compatibility);
                }
            }
        }
    }
}

void calculate_single_thread(const string &input_path, const string &output_path,const unsigned int number_of_pieces, const bool debug){

    // create array of piece shape
    PieceConnection::set_number_of_pieces(number_of_pieces);
    PieceShape::set_origin_path(input_path);
    PieceShape pieces_shapes[number_of_pieces];

    // create array of piece logic
    PieceConnection pieces_connections[number_of_pieces];

    // filling both array up with the respective index;
    for(int i=0; i < number_of_pieces; i++){
        pieces_connections[i].became(i);
        pieces_shapes[i] = PieceShape(i);
    }

    auto start = chrono::steady_clock::now();

    // compare all the pieces_shapes one by one, and save the results in the piece logic
    for(int piece_id=0; piece_id < number_of_pieces; piece_id++){
        cout << "done piece: " << piece_id << "/" << number_of_pieces << endl;
        for(int piece_side=0; piece_side<4; piece_side++){

            // if this side is a border i don't need to compare it with others
            //if(pieces_shapes[piece_id].get_side(piece_side).get_kind() == BORDER){
            //    continue;
            //}

            for(int other_piece_id=piece_id+1; other_piece_id < number_of_pieces; other_piece_id++){
                for(int other_piece_side=0; other_piece_side<4; other_piece_side++){

                    // check for compatibility
                    float compatibility = pieces_shapes[piece_id].get_side(piece_side).compare_to(
                            pieces_shapes[other_piece_id].get_side(other_piece_side)
                    );
                    if(compatibility < MIN_COMPAT_CC){
                        compatibility = 0;
                    }
                    if(PreviewManager::is_preview_enabled()){
                        Mat img = pieces_shapes[piece_id].get_side(piece_side).get_compare_image(
                                pieces_shapes[other_piece_id].get_side(other_piece_side)
                                );
                        PreviewManager::output_preview_image(img);
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

    auto end = chrono::steady_clock::now();

    cout << "Execution time in seconds [single threaded]: "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec" << endl;

    // save the connections information to the disk
    for(auto & i : pieces_connections){
        i.save_as_file(output_path);
    }

}


void calculate_multi_thread(const string &input_path, const string &output_path,unsigned int number_of_cores,const unsigned int number_of_pieces){
    PieceConnection::set_number_of_pieces(number_of_pieces);

    // create array of piece shape
    PieceShape::set_origin_path(input_path);
    PieceShape* pieces_shapes = new PieceShape[number_of_pieces];

    // create array of piece logic
    PieceConnection* pieces_logic = new PieceConnection[number_of_pieces];

    // filling both array up with the respective index;
    for(int i=0; i < number_of_pieces; i++){
        pieces_logic[i].became(i);
        pieces_shapes[i] = PieceShape(i);
    }

    // create a shared index
    atomic<int> index = 0;

    // if the number fo cores is zero: automatic detection
    unsigned  int processor_count;
    if(number_of_cores == 0){
        // find how many cores are available
        processor_count = std::thread::hardware_concurrency();
    }else{
        processor_count = number_of_cores;
    }



    // make share the number is detected correctly
    assert(processor_count != 0);

    // array of pointers to thread
    auto *threads = new thread[processor_count];


    auto start = chrono::steady_clock::now();

    // launch all threads
    for(int i=0; i<processor_count; i++){
        threads[i] = thread(piece_comparer_thread,pieces_logic,pieces_shapes,number_of_pieces, &index);
    }

    // showing debug
    while (index < number_of_pieces){
        cout << "processing piece: " << index << "/" << number_of_pieces << endl;
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
    for(int i=0; i<number_of_pieces; i++){
        //cout << pieces_logic[i].to_string() << endl;
        pieces_logic[i].save_as_file(output_path);
    }

    delete[] pieces_shapes;
    delete[] pieces_logic;
}

