use crate::piece_array::PieceArray;
use crate::piece_group::{CanCreateSet, GroupCreationResult, HasKnownLevel, IsSubComponent, PieceArrayFiller,NextLevelOrPanic};
use crate::piece_group::PieceGroup;
use crate::piece_group::Comparable;
use crate::piece_group::HasOrientation;
use crate::piece_group_holder::PieceGroupHolder;
use std::collections::LinkedList;
use std::sync::Mutex;
use rayon::prelude::*;
use crate::constants::MIN_SHORE_PIECE_ARRAY;
use crate::finalize_piece_array::finalize_piece_array;

pub const TEST: bool = true;

pub fn solve<T: NextLevelOrPanic>(pgh: &PieceGroupHolder<T>, output_path: &str, size_x: u64, size_y: u64) -> bool{

    println!("The level {} has started",T::LEVEL);

    let size = pgh.get_size();

    // create the list for the output pieces
    let mut output_vec = Mutex::new(LinkedList::<PieceGroup<T>>::new());

    // the function to apply for every iteration of the loop
    let solve_lambda_function = |top_left_index: usize| {

        // select all 3 other components one by one. start from top_left_index +1 to avoid duplicates
        'top_right_index_loop:
        for top_right_index in top_left_index..size{
            'bottom_right_index_loop:
            for bottom_right_index in top_left_index..size{
                'bottom_left_index_loop:
                for bottom_left_index in top_left_index..size{

                    // keep track of how many times i fail the placement of the piece,
                    // if for examole piece 1 and piece 2 are incompatible as top right and top left, in any possible combination
                    // then there is no need to test all combinations of bottom right and bottom left
                    let mut failed_top_right_placement = 0;
                    let mut failed_bottom_right_placement = 0;

                    'top_left_orientation_loop:
                    for top_left_orientation in 0..4{
                        'top_right_orientation_loop:
                        for top_right_orientation in 0..4{
                            'bottom_right_orientation_loop:
                            for bottom_right_orientation in 0..4{
                                'bottom_left_orientation_loop:
                                for bottom_left_orientation in 0..4{

                                    // get the 4 element our sub piece will be made of
                                    let top_left= pgh.get(top_left_index,top_left_orientation);
                                    let top_right= pgh.get(top_right_index,top_right_orientation);
                                    let bottom_right= pgh.get(bottom_right_index,bottom_right_orientation);
                                    let bottom_left= pgh.get(bottom_left_index,bottom_left_orientation);

                                    // create a super piece with the 4 sub piece
                                    let pg = T::merge_together(top_left,top_right,bottom_right,bottom_left);

                                    // based on the
                                    let pg = match pg {
                                        // the piece group is a good possible combination, so i add it to the list of options
                                        GroupCreationResult::Ok(e) => e,
                                        // the piece group has an avrege shore that is lower then the minimum threshold,
                                        // so i do not add it to the list and go on with the next combination
                                        GroupCreationResult::AvregeIsTooLow => {
                                            //println!("skip because of: AvregeIsTooLow");
                                            continue;
                                        }
                                        // the piece is an impossible combination (because one of the pieces is repeated)
                                        // so i skip to the next iteration of his loop
                                        GroupCreationResult::TopRightImpossibleCombination => {
                                            //println!("skip because of: TopRightImpossibleCombination");
                                            continue 'top_right_index_loop
                                        }
                                        GroupCreationResult::BottomLeftImpossibleCombination => {
                                            //println!("skip because of: BottomLeftImpossibleCombination");
                                            continue 'bottom_left_index_loop
                                        }
                                        GroupCreationResult::BottomRightImpossibleCombination => {
                                            //println!("skip because of: BottomRightImpossibleCombination");
                                            continue 'bottom_right_index_loop
                                        }
                                        GroupCreationResult::TopRightImpossibleFit => {
                                            failed_top_right_placement += 1;
                                            failed_bottom_right_placement += 4;
                                            // if the top right piece dose not fit 16 times
                                            // (all the possible orientation of the top right and top left has been tested
                                            // witch means that the second piece is no longer an option
                                            if failed_top_right_placement >= 16{
                                                //println!("skip because of: TopRightImpossibleFit-1");
                                                continue 'top_right_index_loop
                                            }

                                            // if the above condition is not met, just test a new orientation
                                            //println!("skip because of: TopRightImpossibleFit-0");
                                            continue 'top_right_orientation_loop
                                        }
                                        GroupCreationResult::BottomLeftImpossibleFit => {
                                            failed_bottom_right_placement += 1;

                                            if failed_top_right_placement >= 64{
                                                //println!("skip because of: BottomLeftImpossibleFit-1");
                                                continue 'bottom_left_index_loop

                                            }

                                            // if the above condition is not met, just test a new orientation
                                            //println!("skip because of: BottomLeftImpossibleFit-0");
                                            continue 'bottom_left_orientation_loop
                                        }
                                        GroupCreationResult::BottomRightImpossibleFit => {
                                            // todo forse devo incrementare uno dei contatori?
                                            //println!("skip because of: BottomRightImpossibleFit");
                                            continue 'bottom_right_orientation_loop
                                        }
                                    };

                                    // now it is possible to check if the pieces match graphically, calling the c++ func

                                    // create the piece array with the appropriate size
                                    let mut pa = PieceArray::new(
                                        PieceGroup::<T>::SIDE_LEN,
                                        PieceGroup::<T>::SIDE_LEN
                                    );

                                    // fill the piece array with the piece group
                                    pg.fill_piece_array(&mut pa,0,0,0);

                                    // calling the c++ dll
                                    unsafe {
                                        let mut paw = pa.get_piece_array_wrapper();

                                        // if the shore is to low i continue on the next iteration
                                        if (*paw).get_shore() < MIN_SHORE_PIECE_ARRAY{
                                            //println!("skip because of: MIN_SHORE_PIECE_ARRAY: {}",(*paw).get_shore());
                                            (*paw).destroy_piece_array_wrapper();
                                            continue;
                                        }
                                        // send the prevew image to the c# backend
                                        (*paw).send_preview_image();
                                        // deallocate memory
                                        (*paw).destroy_piece_array_wrapper();
                                    }

                                    // add the element to the list
                                    output_vec.lock().unwrap().push_back(pg);
                                }
                            }
                        }
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

    // create the new piece group holder
    let pgh_out = PieceGroupHolder::new(output_vec);

    // call the next iteration of the current function, and panic if it has not been compiled
    // the limit is 10 iterations... or a 4M pieces puzzle
    return T::next_or_panic(&pgh_out, output_path, size_x, size_y);
}