
use crate::piece_array::PieceArray;
use crate::piece_group::{HasBasicComponents, GroupCreationResult, HasKnownLevel, IsSubComponent, PieceArrayFiller, NextLevelOrPanic, Direction};
use crate::piece_group::PieceGroup;
use crate::piece_group::Comparable;
use crate::piece_group::HasOrientation;
use crate::piece_group_holder::{MatchForAllPieces, PieceGroupHolder};
use std::collections::LinkedList;
use std::fmt::Debug;
use std::sync::Mutex;
use rayon::prelude::*;
use crate::CALL_BACK_FUNC;
use crate::constants::MIN_SHORE_PIECE_ARRAY;
use crate::finalize_piece_array::finalize_piece_array;
use crate::piece_group::Direction::RIGHT;
use crate::piece_group::GroupCreationResult::BottomRightImpossibleFit;

pub const TEST: bool = true;

pub fn solve<T: NextLevelOrPanic + Debug>(pgh: &PieceGroupHolder<T>, output_path: &str, size_x: u64, size_y: u64) -> bool{

    println!("The level {} has started",T::LEVEL);

    let size = pgh.get_size();

    let match_for_all_pieces = MatchForAllPieces::new(&pgh);

    // create the list for the output pieces
    let mut output_vec = Mutex::new(LinkedList::<PieceGroup<T>>::new());

    // the function to apply for every iteration of the loop
    let solve_lambda_function = |top_left_index: usize| {

        'top_left_orientation_loop:
        for top_left_orientation in 0..4 {
            let top_left = pgh.get(top_left_index, top_left_orientation);

            'top_right_loop:
            for top_right in match_for_all_pieces.get_matches_with_higher_index_explicit(top_left_index,top_left_orientation,Direction::RIGHT,top_left_index){

                'bottom_right_loop:
                for bottom_right in match_for_all_pieces.get_matches_with_higher_index(top_right, Direction::DOWN,top_left_index){


                    'bottom_left_loop:
                    for bottom_left in match_for_all_pieces.get_matches_with_higher_index(bottom_right, Direction::LEFT,top_left_index){

                        // create a super piece with the 4 sub piece
                        let pgr = T::merge_together(top_left, top_right, bottom_right, bottom_left);

                        // based on the
                        let pg = match pgr {
                            // the piece group is a good possible combination, so i add it to the list of options
                            GroupCreationResult::Ok(e) => e,
                            GroupCreationResult::TopRightImpossibleCombination => continue 'top_right_loop,
                            GroupCreationResult::BottomRightImpossibleCombination => continue 'bottom_left_loop,
                            GroupCreationResult::BottomLeftImpossibleCombination => continue 'bottom_left_loop,
                            GroupCreationResult::BottomLeftImpossibleFit => continue 'bottom_left_loop,
                            GroupCreationResult::AvregeIsTooLow => continue,
                            _ => panic!("unexpected error!")
                        };

                        // now it is possible to check if the pieces match graphically, calling the c++ func

                        // create the piece array with the appropriate size
                        let mut pa = PieceArray::new(
                            PieceGroup::<T>::SIDE_LEN,
                            PieceGroup::<T>::SIDE_LEN
                        );

                        // fill the piece array with the piece group
                        pg.fill_piece_array(&mut pa, 0, 0, 0);

                        // calling the c++ dll
                        unsafe {
                            let mut paw = pa.get_piece_array_wrapper();

                            // if the shore is to low i continue on the next iteration

                            if (*paw).get_shore() < MIN_SHORE_PIECE_ARRAY {
                                //println!("skip because of: MIN_SHORE_PIECE_ARRAY: {}",(*paw).get_shore());
                                (*paw).destroy_piece_array_wrapper();
                                continue;
                            }
                            //println!("{:?}",pg);
                            // send the prevew image to the c# backend
                            CALL_BACK_FUNC(paw);
                            // deallocate memory
                            (*paw).destroy_piece_array_wrapper();
                        }

                        // add the element to the list
                        output_vec.lock().unwrap().push_back(pg);

                    }

                }

            }

        }
        println!("finish: {top_left_index}");
    };


    (0..size).into_par_iter().for_each(
        |x| solve_lambda_function(x)
    );

    // convert list to vector
    let output_vec: Vec<PieceGroup<T>> = output_vec.lock().unwrap().iter().map(|x| x.clone()).collect();

    println!("I found {} pieces",output_vec.len());
    //panic!();
    // create the new piece group holder
    let pgh_out = PieceGroupHolder::new(output_vec);

    // call the next iteration of the current function, and panic if it has not been compiled
    // the limit is 10 iterations... or a 4M pieces puzzle
    T::next_or_panic(&pgh_out, output_path, size_x, size_y)
}