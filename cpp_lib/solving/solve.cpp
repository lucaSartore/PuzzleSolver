
#include "solve.h"

#include <iostream>
#include "graphic_piece/Side.h"
#include "graphic_piece/PieceShape.h"
#include "graphic_piece/util_piece.h"
#include "logic_piece/PieceConnection.h"
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include <utility>
#include "groped_pieces/GroupedPieces.h"
#include "puzzle_preview/PieceArray.h"
#include "groped_pieces/grouped_pieces_errors.h"
#include "groped_pieces/GroupedPiecesHolder.h"
#include "constants.h"
#include "puzzle_preview/PreviewManager.h"
/// recursive solving function:
/// take as input:
/// - the x and y dimension of the desired piece,
/// - an holder with all the pieces
/// - the number of cores to use
/// - a pointer to all the images of the puzzle for better evaluation
/// - the path where to save the fina result
template<int N>
void solve_recursive(
        unsigned int dim_x,
        unsigned int dim_y,
        GroupedPiecesHolder<N> &input_pieces,
        unsigned int number_of_cores,
        PieceImage* images,
        string output_path
        );


/// thread spawned by solve_recursive to find the possible combinations
template<int N>
void solve_thread(GroupedPiecesHolder<N> *input_pieces,
                  atomic<int> * index,
                  atomic<int> * found,
                  std::list<GroupedPieces<N+1>> *results_list,
                  mutex *result_list_mutex,
                  PieceImage* images
                  );



void solve_puzzle_function(
        std::string input_path_connections,
        std::string input_path_images,
        std::string output_path,
        unsigned int dim_x,
        unsigned int dim_y,
        unsigned int number_of_pieces,
        unsigned int number_of_cores
){
    // create tee array of images and connections
    PieceConnection::set_number_of_pieces(number_of_pieces);
    PieceConnection*  pieces_connection = new PieceConnection[number_of_pieces];
    PieceImage*  piece_images = new PieceImage[number_of_pieces];


    // load all the pieces
    for(int i=0; i < number_of_pieces; i++){
        pieces_connection[i].became(input_path_connections,i);
        piece_images[i] = PieceImage(i,input_path_images);
    }


//    cout << "CMP: " << pieces_connection[1].compare(2,3,3) << endl;

    // creating the list of level 1 pieces;
    list<GroupedPieces<1>> initial_list = {};
    for(int i=0; i < number_of_pieces; i++){
        initial_list.emplace_back(pieces_connection+i);
    }

    // creating the holder for the group
    GroupedPiecesHolder<1> initial_group = GroupedPiecesHolder(initial_list);

    // launch the recursive function

    //solve_recursive<1>(dim_x,dim_y,initial_group,number_of_pieces,piece_images);
    solve_recursive<1>(dim_x,dim_y,initial_group,number_of_cores,piece_images,std::move(output_path));


    // free the memory
    delete[] pieces_connection;
    delete[] piece_images;
}


/// at a point we must end the recursion (otherwise the compiler wont stop)
template<>
void solve_recursive<MAX_GroupedPieces_LEVEL>(unsigned int dim_x,unsigned int dim_y,GroupedPiecesHolder<MAX_GroupedPieces_LEVEL> &input_pieces,unsigned int number_of_cores,PieceImage* images, string output_path){
    throw runtime_error("Maximum dimension reached");
}


template<int N>
void solve_recursive(unsigned int dim_x,unsigned int dim_y,GroupedPiecesHolder<N> &input_pieces,unsigned int number_of_cores,PieceImage* images, string output_path){

    // make shore we are using at least 4 cores
    assert(number_of_cores != 0);

    // index for the first piece loop
    atomic<int> index = 0;
    // index of how many combination has been found
    atomic<int> found = 0;

    // create a list for the obtained pieces
    list<GroupedPieces<N+1>> result_list = {};
    mutex result_list_mutex;

    // create teh correct amount of threads
    thread* threads = new thread[number_of_cores];

    //spawn the threads
    for(int i=0; i<number_of_cores; i++){
        threads[i] = thread(solve_thread<N>, &input_pieces, &index, &found,&result_list,&result_list_mutex,images);
    }

    int number_of_pieces = input_pieces.get_length();

    do{
        float percent = (float)index/(float)number_of_pieces*100;
        cout << "step " << N << ": " << index<<"/" <<number_of_pieces << endl;
        cout << found << " valid combination found" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds (100));
    }while (index < number_of_pieces);
    float percent = (float)index/(float)number_of_pieces*100;
    cout << "step " << N << ": " << index<<"/" <<number_of_pieces << endl;
    cout << found << " valid combination found" << endl;

    //join all threads
    for(int i=0; i<number_of_cores; i++){
        threads[i].join();
    }

    // free memory
    delete[] threads;

    // the current length of the side of the pieces found in resul_list
    int side_len = std::round(pow(2.0,N));

    // reach the end, now need to do final assembly
    if(side_len*2 > std::min(dim_x,dim_y)){
        //todo: add compatibility for all puzzle shape

        // save the best result

        // find best result
        int best_index,  i=0;
        float best_shore = 0;
        for(auto solution: result_list){
            auto pa = solution.get_piece_array(images);

            float shore = pa.get_shore();
            if(shore > best_shore){
                best_shore = shore;
                best_index = i;
            }
            i++;
        }

        i=0;
        // save the result at the best index
        for(auto solution: result_list){

            if(i == best_index){
                auto pa = solution.get_piece_array(images);
                imwrite(output_path+"solution.png",pa.get_image(PREVIEW));
                pa.save_as_file(output_path+"solution.bin");

                //imshow("solution", pa.get_image(PREVIEW));
                //waitKey(0);

                break;
            }
            i++;
        }
    }
    // not yer reach the end, make recursive case
    else{
        // creating an holder fot the newly found pieces
        GroupedPiecesHolder<N+1> new_group_pieces = GroupedPiecesHolder<N+1>(result_list);

        // call the recursive function
        solve_recursive<N+1>(dim_x,dim_y,new_group_pieces,number_of_cores,images,output_path);
    }
}


template<int N>
void solve_thread(GroupedPiecesHolder<N> *input_pieces, atomic<int> * index, atomic<int> * found, std::list<GroupedPieces<N+1>> *result_list, mutex *result_list_mutex, PieceImage* images){
    /*if(N == 2){
        cout << "EXITING" << endl;
        exit(69);
    }*/
    // calculate total length
    int number_of_pieces = input_pieces->get_length();

    // the index of the top left piece of the sub component
    int top_left_index;
    while (true){
        // get the index and then add 1;
        top_left_index  = std::atomic_fetch_add(index, 1);
        //cout << "increment: " << top_left_index  << "   " << *index << endl;

        // if reached the end, return
        if(top_left_index >= number_of_pieces){
            return;
        }

        // select all 3 other components one by one. start from top_left_index +1 to avoid duplicates
        for(int top_right_index = top_left_index+1; top_right_index<number_of_pieces; top_right_index++){
            for (int bottom_right_index = top_left_index + 1;bottom_right_index < number_of_pieces; bottom_right_index++) {
                for (int bottom_left_index = top_left_index + 1;bottom_left_index < number_of_pieces; bottom_left_index++) {

                    // keep track of how many times i fail the placement of the piece,
                    // if for examole piece 1 and piece 2 are incompatible as top right and top left, in any possible combination
                    // then there is no need to test all combinations of bottom right and bottom left
                    int failed_top_right_placement = 0;
                    int failed_bottom_right_placement = 0;

                    // select the possible orientations for each component
                    for (int top_left_orientation = 0; top_left_orientation < 4; top_left_orientation++) {
                        for (int top_right_orientation = 0; top_right_orientation < 4; top_right_orientation++) {
                            for (int bottom_right_orientation = 0;bottom_right_orientation < 4; bottom_right_orientation++) {
                                for (int bottom_left_orientation = 0;bottom_left_orientation < 4; bottom_left_orientation++) {

                                    auto top_left = &input_pieces->get(top_left_index, top_left_orientation);
                                    auto top_right = &input_pieces->get(top_right_index, top_right_orientation);
                                    auto bottom_right = &input_pieces->get(bottom_right_index, bottom_right_orientation);
                                    auto bottom_left = &input_pieces->get(bottom_left_index, bottom_left_orientation);

                                    // try to create the piece, this can create an error if they are not compatible,
                                    // based of where, in the creation, the error occurs i can skip some of the inner for loops
                                    try {
                                        // create new piece
                                        auto new_piece = GroupedPieces<N+1>(top_left, top_right, bottom_right, bottom_left);

                                        // create the array
                                        auto array = new_piece.get_piece_array(images);

                                        // get the shore of the array
                                        if(array.get_shore()>MIN_SHORE_PIECE_ARRAY){

                                            // print the image
                                            if(PreviewManager::is_preview_enabled()){
                                                Mat image = array.get_image(PREVIEW);
                                                crop_image_to_remove_black_gb(image);
                                                PreviewManager::output_preview_image(image);
                                            }

                                            //assert(!debug_flag);

                                            // if shore is high enough, add into list
                                            result_list_mutex->lock();
                                            result_list->push_back(std::move(new_piece));

                                            // write the file to the disk
                                            string output_path = std::format(
                                                "{}_{}_{}_{}_{}_{}_{}_{}.png",
                                                top_left_index , top_right_index,bottom_right_index,bottom_left_index,
                                                top_left_orientation,top_right_orientation,bottom_right_orientation,bottom_left_orientation
                                            );
                                            imwrite(output_path,array.get_image(PREVIEW));

                                            result_list_mutex->unlock();

                                            // update tee number of found pieces
                                            std::atomic_fetch_add(found, 1);
                                        }

                                    } catch (AvregeIsToLow &e) {
                                        //cout << "AvregeIsToLow" << endl;
                                        // if avrege is to low: do nothing and go on with the next piece
                                    } catch (BottomLeftImpossibleFit &e) {
                                        //cout << "BottomLeftImpossibleFit" << endl;
                                        // if bottom left is impossible: do nothing and go on with the next piece
                                    } catch (BottomRightImpossibleFit &e) {
                                        //cout << "BottomRightImpossibleFit" << endl;

                                        // keep track of how many times i fail
                                        failed_bottom_right_placement += 1;

                                        // if all combination of top right are impossible: no need to check all bottom left combinations
                                        if(failed_top_right_placement == 64){
                                            goto EXIT_BOTTOM_LEFT_INDEX_LOOP;
                                        }
                                        // if this is not the last possible combination: break the orientation loop
                                        goto EXIT_BOTTOM_LEFT_ORIENTATION_LOOP;


                                    } catch (TopRightImpossibleFit &e) {
                                        //cout << "TopRightImpossibleFit" << endl;

                                        // keep track of how many times i fail
                                        failed_top_right_placement +=1;
                                        // need to cont this 4 times
                                        failed_bottom_right_placement +=4;

                                        // if all combination of top right are impossible: no need to check all bottom left combinations
                                        if(failed_top_right_placement == 16){
                                            goto EXIT_BOTTOM_RIGHT_INDEX_LOOP;
                                        }

                                        // if this is not the last possible combination: break the orientation loop
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
    }

}


/*
PieceImage* piece_image_global;
int old_main(){

    int number_of_pieces = 2;
    //calculate_single_thread(); return 0;

    string path = "../dataset/test_8x8/connections";

    PieceConnection::set_number_of_pieces(number_of_pieces);
    PieceConnection pieces[number_of_pieces];
    PieceImage piece_images[number_of_pieces];

    piece_image_global = piece_images;


    // load all the pieces
    for(int i=0; i < number_of_pieces; i++){
        pieces[i].became(path,i);
        piece_images[i] = PieceImage(i, "../dataset/test_8x8/divided");
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
    number_of_pieces = group_level_1.get_length();
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



    //removing elements that do not met the minimum percentage

    auto remove_condition = [&piece_images](GroupedPieces<2> group) { return group.template get_piece_array<ShoringHolder>(piece_images).get_shore() < 0.96; };

    // Use std::remove_if to filter the list
    list_lev_2.erase(std::remove_if(list_lev_2.begin(), list_lev_2.end(), remove_condition), list_lev_2.end());

    cout << "new_lenght: " << list_lev_2.size() << endl;

    //for(auto e: list_lev_2){auto pa = e.get_piece_array<PreviewHolder>(piece_images);imshow("a",pa.get_preview_image());waitKey(0);}

    // creating the list with rotated elements
    GroupedPiecesHolder<2> group_level_2 = GroupedPiecesHolder(list_lev_2);


    cout << "let's gooo!!!" << endl;


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
                                for (int bottom_left_orientation = 0; bottom_left_orientation < 4; bottom_left_orientation++) {

                                    auto top_left = &group_level_2.get(top_left_index, top_left_orientation);
                                    auto top_right = &group_level_2.get(top_right_index, top_right_orientation);
                                    auto bottom_right = &group_level_2.get(bottom_right_index, bottom_right_orientation);
                                    auto bottom_left = &group_level_2.get(bottom_left_index, bottom_left_orientation);

                                    try {
                                        list_lev_3.emplace_front(top_left, top_right, bottom_right, bottom_left);
                                    } catch (AvregeIsToLow &e) {
                                        //cout << "AvregeIsToLow" << endl;waitKey(0);
                                        // if avrege is to low: do nothing and go on with the next piece
                                    } catch (BottomLeftImpossibleFit &e) {
                                        //cout << "BottomLeftImpossibleFit" << endl;waitKey(0);
                                        // if bottom left is impossible: do nothing and go on with the next piece
                                    } catch (BottomRightImpossibleFit &e) {
                                        //cout << "BottomRightImpossibleFit" << endl;waitKey(0);
                                        // if bottom right is impossible: no need to check all bottom left combinations
                                        // so i jump out of the bottom left orientation loop
                                        goto EXIT_BOTTOM_LEFT_ORIENTATION_LOOP2;
                                    } catch (TopRightImpossibleFit &e) {
                                        //cout << "TopRightImpossibleFit" << endl;waitKey(0);
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
        imshow("test", pa.get_image());
        waitKey(0);
    }

    return 0;
}*/
