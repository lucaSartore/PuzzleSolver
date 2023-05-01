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
#include "puzzle/PieceArray.h"
#include <time.h>
#include "groped_pieces/grouped_pieces_errors.h"
#include "groped_pieces/GroupedPiecesHolder.h"

using namespace std;
using namespace std::chrono;

#define NUMBER_OF_PIECES 16
#define MINIMUM_COMPATIBILITY_PERCENTAGE 0.25


void piece_comparer_thread(PieceConnection pieces_connections[], PieceShape pieces_shapes[], atomic<int> *index);
void calculate_single_thread(bool debug = false);
void calculate_multi_thread(int number_of_threads = 0);
void test_piece_array();
void test_piece_array_shore();
void test_grouped_piece_2_constructor();
void test_grouped_piece_holder();

int main(){

    //srand(time(NULL));

    //test_piece_array(); return 0;
    //test_piece_array_shore(); return 0;
    //test_grouped_piece_2_constructor(); return 0;
    //calculate_single_thread();return 0;
    //test_grouped_piece_holder(); return 0;

    string path = "../../dataset/test_4x4/connections";

    PieceConnection::set_number_of_pieces(NUMBER_OF_PIECES);
    PieceConnection pieces[NUMBER_OF_PIECES];
    PieceImage piece_images[NUMBER_OF_PIECES];


    // load all the pieces
    for(int i=0; i<NUMBER_OF_PIECES; i++){
        pieces[i].became(path,i);
        piece_images[i] = PieceImage(i, "../../dataset/test_4x4/divided");
    }

    // empty list of element 1;
    std::list<GroupedPieces<1>> list_lev_1 = {};

    // creating list with elements of size 1;
    for(auto &piece: pieces){
        list_lev_1.emplace_back(&piece);
    }

    // creating the list with rotated elements
    GroupedPiecesHolder<1> group_level_1 = GroupedPiecesHolder(list_lev_1);

    // empty list of element 2;
    std::list<GroupedPieces<2>> list_lev_2 = {};

    auto start = chrono::steady_clock::now();

    int c=0;
    int number_of_pieces = group_level_1.get_length();
    // top left element get chosen from all pieces
    for(int top_left_index=0; top_left_index<number_of_pieces; top_left_index++){


        // rop right element get chosen from all pieces that are grater then the top left in the list order (to avoid duplicates)
        for(int top_right_index = top_left_index+1; top_right_index<number_of_pieces; top_right_index++){
            // bottom right element get chosen from all pieces that are grater then the top left in the list order (to avoid duplicates)
            for (int bottom_right_index = top_left_index + 1;bottom_right_index < number_of_pieces; bottom_right_index++) {
                // bottom left element get chosen from all pieces that are grater then the top left in the list order (to avoid duplicates)
                for (int bottom_left_index = top_left_index + 1;bottom_left_index < number_of_pieces; bottom_left_index++) {
                    for (int top_left_orientation = 0; top_left_orientation < 4; top_left_orientation++) {
                        // top left 4 possible orientations
                        for (int top_right_orientation = 0; top_right_orientation < 4; top_right_orientation++) {
                            // bottom right 4 possible orientations
                            for (int bottom_right_orientation = 0;bottom_right_orientation < 4; bottom_right_orientation++) {
                                // bottom left 4 possible orientations
                                for (int bottom_left_orientation = 0;
                                     bottom_left_orientation < 4; bottom_left_orientation++) {

                                    //cout << top_left_index << endl;
                                    //cout << top_right_index << " " << top_right_orientation << endl;
                                    //cout << bottom_right_index << " " << bottom_right_orientation << endl;
                                    //cout << bottom_left_index << " " << bottom_left_orientation << endl;


                                    auto top_left = &group_level_1.get(top_left_index, top_left_orientation);
                                    auto top_right = &group_level_1.get(top_right_index, top_right_orientation);
                                    auto bottom_right = &group_level_1.get(bottom_right_index, bottom_right_orientation);
                                    auto bottom_left = &group_level_1.get(bottom_left_index, bottom_left_orientation);

                                    try {
                                        list_lev_2.emplace_front(top_left, top_right, bottom_right, bottom_left);
                                    } catch (AvregeIsToLow &e) {
                                        //cout << "AvregeIsToLow" << endl;
                                        // if avrege is to low: do nothing and go on with the next piece
                                    } catch (BottomLeftImpossibleFit &e) {
                                        //cout << "BottomLeftImpossibleFit" << endl;
                                        // if bottom left is impossible: do nothing and go on with the next piece
                                    } catch (BottomRightImpossibleFit &e) {
                                        //cout << "BottomRightImpossibleFit" << endl;
                                        // if bottom right is impossible: no need to check all bottom left combinations
                                        // so i jump out of the bottom left orientation loop
                                        goto EXIT_BOTTOM_LEFT_ORIENTATION_LOOP;
                                    } catch (TopRightImpossibleFit &e) {
                                        //cout << "TopRightImpossibleFit" << endl;
                                        // if top right is impossible: no need to check all bottom left combinations
                                        // so i jump to the bottom left orientation loop
                                        goto EXIT_BOTTOM_RIGHT_ORIENTATION_LOOP;
                                    } catch (BottomLeftImpossibleCombination &e) {
                                        //cout << "BottomLeftImpossibleCombination" << endl;
                                        // if bottom left is an impossible combination, i don't need to check all his orientations
                                        // so i jump out of the top right orientation loops
                                        goto EXIT_TOP_RIGHT_ORIENTATION_LOOP;
                                    } catch (BottomRightImpossibleCombination &e) {
                                        //cout << "BottomRightImpossibleCombination" << endl;
                                        // if bottom right is an impossible combination, i don't need to check all his orientations
                                        // so i jump out of the bottom left index loops
                                        goto EXIT_BOTTOM_LEFT_INDEX_LOOP;
                                    } catch (TopRightImpossibleCombination &e) {
                                        //cout << "TopRightImpossibleCombination" << endl;
                                        // if top right is an impossible combination, i don't need to check all his orientations
                                        // so i jump out of the bottom right index loops
                                        goto EXIT_BOTTOM_RIGHT_INDEX_LOOP;
                                    }
                                }
                                EXIT_BOTTOM_LEFT_ORIENTATION_LOOP:;
                            }
                            EXIT_BOTTOM_RIGHT_ORIENTATION_LOOP:;
                        }
                        EXIT_TOP_RIGHT_ORIENTATION_LOOP:;
                    }
                }
                EXIT_BOTTOM_LEFT_INDEX_LOOP:;
            }
            EXIT_BOTTOM_RIGHT_INDEX_LOOP:;
        }
        EXIT_TOP_RIGHT_INDEX_LOOP:;
        c++;
        cout << (float) c/(float)number_of_pieces * 100 << "%" << endl;
        cout << list_lev_2.size() << " possible combinations found" << endl;
    }

    auto end = chrono::steady_clock::now();

    cout << "Execution time for finding all 2x2 pieces [single threaded]: "
         << chrono::duration_cast<chrono::seconds>(end - start).count() // baseline: 100 sec // 236 comb
         << " sec" << endl;


    /*for(auto elem: list_lev_2){
        for(auto id: elem.get_ids()){
            cout << id << endl;
        }
        auto pa = elem.get_piece_array<PreviewHolder>(piece_images);
        imshow("a",pa.get_preview_image());
        waitKey(0);
    }*/



    //removing elements that do not met the minimum percentage

    auto remove_condition = [&piece_images](GroupedPieces<2> group) { return group.template get_piece_array<ShoringHolder>(piece_images).get_shore() < 0.96; };

    // Use std::remove_if to filter the list
    list_lev_2.erase(std::remove_if(list_lev_2.begin(), list_lev_2.end(), remove_condition), list_lev_2.end());

    cout << "new_lenght: " << list_lev_2.size() << endl;

    // creating the list with rotated elements
    GroupedPiecesHolder<2> group_level_2 = GroupedPiecesHolder(list_lev_2);

    // empty list of element 2;
    std::list<GroupedPieces<3>> list_lev_3 = {};

    start = chrono::steady_clock::now();

    c=0;
    number_of_pieces = group_level_2.get_length();
    // top left element get chosen from all pieces
    for(int top_left_index=0; top_left_index<number_of_pieces; top_left_index++){
        // rop right element get chosen from all pieces that are grater then the top left in the list order (to avoid duplicates)
        for(int top_right_index = top_left_index+1; top_right_index<number_of_pieces; top_right_index++){
            // bottom right element get chosen from all pieces that are grater then the top left in the list order (to avoid duplicates)
            for (int bottom_right_index = top_left_index + 1;bottom_right_index < number_of_pieces; bottom_right_index++) {
                // bottom left element get chosen from all pieces that are grater then the top left in the list order (to avoid duplicates)
                for (int bottom_left_index = top_left_index + 1;bottom_left_index < number_of_pieces; bottom_left_index++) {
                    for (int top_left_orientation = 0; top_left_orientation < 4; top_left_orientation++) {
                        // top left 4 possible orientations
                        for (int top_right_orientation = 0; top_right_orientation < 4; top_right_orientation++) {
                            // bottom right 4 possible orientations
                            for (int bottom_right_orientation = 0;bottom_right_orientation < 4; bottom_right_orientation++) {
                                // bottom left 4 possible orientations
                                for (int bottom_left_orientation = 0;
                                     bottom_left_orientation < 4; bottom_left_orientation++) {

                                    //cout << top_left_index << endl;
                                    //cout << top_right_index << " " << top_right_orientation << endl;
                                    //cout << bottom_right_index << " " << bottom_right_orientation << endl;
                                    //cout << bottom_left_index << " " << bottom_left_orientation << endl;


                                    auto top_left = &group_level_2.get(top_left_index, top_left_orientation);
                                    auto top_right = &group_level_2.get(top_right_index, top_right_orientation);
                                    auto bottom_right = &group_level_2.get(bottom_right_index, bottom_right_orientation);
                                    auto bottom_left = &group_level_2.get(bottom_left_index, bottom_left_orientation);

                                    try {
                                        list_lev_3.emplace_front(top_left, top_right, bottom_right, bottom_left);
                                    } catch (AvregeIsToLow &e) {
                                        //cout << "AvregeIsToLow" << endl;
                                        // if avrege is to low: do nothing and go on with the next piece
                                    } catch (BottomLeftImpossibleFit &e) {
                                        //cout << "BottomLeftImpossibleFit" << endl;
                                        // if bottom left is impossible: do nothing and go on with the next piece
                                    } catch (BottomRightImpossibleFit &e) {
                                        //cout << "BottomRightImpossibleFit" << endl;
                                        // if bottom right is impossible: no need to check all bottom left combinations
                                        // so i jump out of the bottom left orientation loop
                                        goto EXIT_BOTTOM_LEFT_ORIENTATION_LOOP2;
                                    } catch (TopRightImpossibleFit &e) {
                                        //cout << "TopRightImpossibleFit" << endl;
                                        // if top right is impossible: no need to check all bottom left combinations
                                        // so i jump to the bottom left orientation loop
                                        goto EXIT_BOTTOM_RIGHT_ORIENTATION_LOOP2;
                                    } catch (BottomLeftImpossibleCombination &e) {
                                        //cout << "BottomLeftImpossibleCombination" << endl;
                                        // if bottom left is an impossible combination, i don't need to check all his orientations
                                        // so i jump out of the top right orientation loops
                                        goto EXIT_TOP_RIGHT_ORIENTATION_LOOP2;
                                    } catch (BottomRightImpossibleCombination &e) {
                                        //cout << "BottomRightImpossibleCombination" << endl;
                                        // if bottom right is an impossible combination, i don't need to check all his orientations
                                        // so i jump out of the bottom left index loops
                                        goto EXIT_BOTTOM_LEFT_INDEX_LOOP2;
                                    } catch (TopRightImpossibleCombination &e) {
                                        //cout << "TopRightImpossibleCombination" << endl;
                                        // if top right is an impossible combination, i don't need to check all his orientations
                                        // so i jump out of the bottom right index loops
                                        goto EXIT_BOTTOM_RIGHT_INDEX_LOOP2;
                                    }
                                }
                                EXIT_BOTTOM_LEFT_ORIENTATION_LOOP2:;
                            }
                            EXIT_BOTTOM_RIGHT_ORIENTATION_LOOP2:;
                        }
                        EXIT_TOP_RIGHT_ORIENTATION_LOOP2:;
                    }
                }
                EXIT_BOTTOM_LEFT_INDEX_LOOP2:;
            }
            EXIT_BOTTOM_RIGHT_INDEX_LOOP2:;
        }
        EXIT_TOP_RIGHT_INDEX_LOOP2:;
        c++;
        cout << (float) c/(float)number_of_pieces * 100 << "%" << endl;
        cout << list_lev_3.size() << " possible combinations found" << endl;
    }

    end = chrono::steady_clock::now();

    cout << "Execution time for finding all 2x2 pieces [single threaded]: "
         << chrono::duration_cast<chrono::seconds>(end - start).count() // baseline: 100 sec // 236 comb
         << " sec" << endl;


    for(auto component: list_lev_3){
        PieceArray<PreviewHolder> pa = component.get_piece_array<PreviewHolder>(piece_images);
        imshow("test", pa.get_preview_image());
        waitKey(0);
    }



    return 0;

    //calculate_single_thread();
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

    string path = "../../dataset/test_4x4";

    // create array of piece shape
    PieceConnection::set_number_of_pieces(NUMBER_OF_PIECES);
    PieceShape::set_origin_path(path + "/divided");
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
    int c=0;
    for(auto & i : pieces_connections){
        i.save_as_file(path + "/connections");
        PieceConnection temp = PieceConnection();
        temp.became(path + "/connections",c);
        assert(temp.to_string() == i.to_string());
        cout << i.to_string() << endl;
        cout << temp.to_string() << endl;
        c++;
    }

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
        cout << i.to_string() << endl;
        //i.save_as_file("../../dataset/blue_500pcs/connections");
    }
}

void test_piece_array(){

    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];


    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray<PreviewHolder> pa = PieceArray<PreviewHolder>();

    PreviewHolder base = PreviewHolder(&pieces_images[4], 0);
    pa.set(0,0,std::move(base));


    //imshow("puzzle", pa.get_preview_image());waitKey(0);

    pa.grow_x();

    base = PreviewHolder(&pieces_images[5], 3);pa.set(1, 0, std::move(base));


    //imshow("puzzle", pa.get_preview_image());waitKey(0);

    pa.grow_y();

    base = PreviewHolder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));


    //imshow("puzzle", pa.get_preview_image());waitKey(0);


    //imshow("puzzle", pa.get_preview_image());waitKey(0);

    base = PreviewHolder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));


    PieceArray pa2 = pa;


    pa.attach_bottom(pa2);
    pa.attach_bottom(pa2);

    PieceArray pa3 = pa;

    pa.attach_right(pa3);


    imshow("puzzle", pa.get_preview_image());waitKey(0);
}

void test_grouped_piece_2_constructor(){
    string path = "../../dataset/test_2x3/connections";

    PieceConnection::set_number_of_pieces(6);
    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceConnection pieces[6];
    PieceImage pieces_images[6];


    // load all the pieces
    for(int i=0; i<6; i++){
        pieces[i].became(path,i);
        pieces_images[i] = PieceImage(i);
    }

    GroupedPieces<1> top_left = GroupedPieces<1>(&pieces[4]);
    GroupedPieces<1> top_right = GroupedPieces<1>(&pieces[5]);
    top_right.rotate_by(3);
    GroupedPieces<1> bottom_left = GroupedPieces<1>(&pieces[3]);
    bottom_left.rotate_by(3);
    GroupedPieces<1> bottom_right = GroupedPieces<1>(&pieces[2]);

    GroupedPieces<2> sq =  GroupedPieces<2>(&top_left,&top_right,&bottom_right,&bottom_left);

    auto pa = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa.get_preview_image());waitKey(0);
    imshow("p",pa.get_preview_image());waitKey(0);

    sq.rotate_by(1);

    auto pa2 = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa2.get_preview_image());waitKey(0);
    imshow("p",pa2.get_preview_image());waitKey(0);
}


void test_piece_array_shore(){

    Mat image_og;
    Mat resized;

    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceImage pieces_images[6];


    // filling both array up with the respective index;
    for(int i=0; i<6;i++){
        pieces_images[i] = PieceImage(i);
    }

    PieceArray<ShoringHolder> pa = PieceArray<ShoringHolder>();

    ShoringHolder base = ShoringHolder(&pieces_images[4], 0);
    base.rotate_by(-0.1);
    pa.set(0,0,std::move(base));



    image_og = pa.get_preview_image();
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);

    pa.grow_x();

    base = ShoringHolder(&pieces_images[5], 3);pa.set(1, 0, std::move(base));


    image_og = pa.get_preview_image();
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);


    pa.grow_y();

    base = ShoringHolder(&pieces_images[3], 3);
    pa.set(0,1,std::move(base));

    base = ShoringHolder(&pieces_images[2], 0);
    pa.set(1,1,std::move(base));
    image_og = pa.get_preview_image();

    cout << "shore: " << pa.get_shore() << endl;

    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);

    PieceArray pa2 = pa;


    pa.attach_bottom(pa2);
    //pa.attach_bottom(pa2);

    //PieceArray pa3 = pa;

    //pa.attach_right(pa3);


    cout << "shore: " << pa.get_shore() << endl;

    image_og = pa.get_preview_image();
    resize(image_og,resized,image_og.size()/8);
    imshow("puzzle", resized);waitKey(0);
}

void test_grouped_piece_holder(){
    string path = "../../dataset/test_2x3/connections";
    PieceConnection::set_number_of_pieces(6);
    PieceImage::set_origin_path("../../dataset/test_2x3/divided");
    PieceConnection pieces[6];
    PieceImage pieces_images[6];


    // load all the pieces
    for(int i=0; i<6; i++){
        pieces[i].became(path,i);
        pieces_images[i] = PieceImage(i);
    }

    list<GroupedPieces<1>> list1 = {};

    for(int i=0; i<6; i++){
        list1.emplace_back(pieces+i);
    }

    auto group_1 = GroupedPiecesHolder<1>(list1);

    GroupedPieces<2> sq =  GroupedPieces<2>(
            &group_1.get(4,0),
            &group_1.get(5,3),
            &group_1.get(2,0),
            &group_1.get(3,3)
            );

    auto pa = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa.get_preview_image());waitKey(0);

    sq.rotate_by(1);

    auto pa2 = sq.get_piece_array<PreviewHolder>(pieces_images);
    imshow("p",pa2.get_preview_image());waitKey(0);
}