use std::sync::Mutex;
use libc::remove;
use rayon::prelude::*;
use crate::constants::{MIN_SHORE_PIECE_ARRAY, MIN_SHORE_PIECE_GROUP};
use crate::piece_array::PieceArray;
use crate::piece_group::{Direction, NextLevelOrPanic, PieceGroup};
use crate::piece_group_holder::PieceGroupHolder;

/// this function takes as input a list of piece groups (that by nature have shape N^2 x N^2 with N positive integer)
/// and turns it into a piece array with an arbitrary dimension
/// Note: N^2 * 2 must be greater or equal of both size_x and size_y in order for this function to work
/// puzzle with a wide aspect ration (greater that 2:1) are not supported by this function)
pub fn finalize_piece_array<T: NextLevelOrPanic>(pgh: &PieceGroupHolder<T>, output_path: &str ,size_x: u64, size_y: u64) -> bool{

    let mut candidates = Mutex::new(Vec::<PieceArray>::new());
    println!("{}",T::SIDE_LEN);
    let shrink_size_x =  T::SIDE_LEN*2 - size_x;
    let shrink_size_y = T::SIDE_LEN*2 - size_y;


    let function= |top_left_id: usize|  {
        for top_left_orientation in 0..4{

            let top_left = pgh.get(top_left_id,top_left_orientation);

            for top_right_id in (top_left_id+1)..pgh.get_size(){
                'tr_orientation:
                for top_right_orientation in 0..4{

                    let top_right = pgh.get(top_right_id,top_right_orientation);

                    for bottom_right_id in (top_left_id+1)..pgh.get_size(){
                        'br_orientation:
                        for bottom_right_orientation in 0..4{

                            let bottom_right = pgh.get(bottom_right_id,bottom_right_orientation);

                            for bottom_left_id in (top_left_id+1)..pgh.get_size(){
                                'bl_orientation:
                                for bottom_left_orientation in 0..4{

                                    let bottom_left = pgh.get(bottom_left_id,bottom_left_orientation);

                                    if top_left.compare_to_with_intersection(Direction::RIGHT,
                                                top_right,shrink_size_x).get_shore() < MIN_SHORE_PIECE_GROUP{
                                        continue 'tr_orientation;
                                    }else if false{
                                        let mut pa = PieceArray::new(T::SIDE_LEN,T::SIDE_LEN);
                                        top_left.fill_piece_array(&mut pa,0,0,0);
                                        println!("top left: {:?}",pa);
                                        top_right.fill_piece_array(&mut pa,0,0,0);
                                        println!("top right: {:?}",pa);

                                        let mut pa = PieceArray::new(size_x,T::SIDE_LEN);
                                        //let mut pa = PieceArray::new(T::SIDE_LEN,T::SIDE_LEN);
                                        top_left.fill_piece_array(&mut pa,0,0,0);
                                        top_right.fill_piece_array(&mut pa,T::SIDE_LEN-shrink_size_x,0,0);
                                        println!("merged: {:?}",pa);

                                        unsafe {
                                            // call c++ dll
                                            let mut paw = pa.get_piece_array_wrapper();
                                            (*paw).save_as_binary("test.bin");
                                            // save the image
                                            (*paw).generate_test_image("candidate.png");
                                            println!();
                                        }
                                    }
                                    if top_right.compare_to_with_intersection(Direction::DOWN,
                                                bottom_right,shrink_size_y).get_shore() < MIN_SHORE_PIECE_GROUP{
                                        continue 'br_orientation;
                                    }
                                    if bottom_right.compare_to_with_intersection(Direction::LEFT,
                                                bottom_left,shrink_size_x).get_shore() < MIN_SHORE_PIECE_GROUP{
                                        continue 'bl_orientation;
                                    }
                                    if bottom_left.compare_to_with_intersection(Direction::UP,
                                                top_left,shrink_size_y).get_shore() < MIN_SHORE_PIECE_GROUP{
                                        continue;
                                    }
                                    // todo: control that there are no duplicates


                                    // fill piece array
                                    let mut pa = PieceArray::new(size_x,size_y);
                                    top_left.fill_piece_array(&mut pa,0,0,0);
                                    top_right.fill_piece_array(&mut pa,T::SIDE_LEN-shrink_size_x,0,0);
                                    bottom_left.fill_piece_array(&mut pa,0,T::SIDE_LEN-shrink_size_y,0);
                                    bottom_right.fill_piece_array(&mut pa,T::SIDE_LEN-shrink_size_x,T::SIDE_LEN-shrink_size_y,0);

                                    // add to candidates
                                    candidates.lock().unwrap().push(pa);
                                }
                            }
                        }
                    }
                }
            }
        }
    };

    (0..pgh.get_size()).into_iter().for_each(|top_left_id|{
        function(top_left_id);
    });

    let mut best: Option<PieceArray> = None;
    let best_shore = 0.0;
    // nota: for now this function only solve squared puzzles
    for candidate in candidates.lock().unwrap().iter_mut(){
        // calculate the shore of the current piece
        unsafe {
            // call c++ dll
            let mut paw = candidate.get_piece_array_wrapper();

            // get the shore of the  current piece array
            let shore = (*paw).get_shore();

            // keep track of the best shore
            if shore > best_shore{
                best = Option::Some(candidate.clone());
            }
            // save the image
            (*paw).generate_test_image("candidate.png");
            println!("{:?}",candidate);

            // deallocate memory
            (*paw).destroy_piece_array_wrapper();
        }

    }
    // get the best result, or return false to signal that the puzzle hasn't been solved
    let mut best = match best {
        Option::Some(e) => e,
        Option::None => return false
    };

    println!("{:?}",best);

    // save the connections result
    best.save_actual_connections_to_json(format!("{}\\connections_result.json",output_path).as_str()).unwrap_or_else(|e| println!("print connections to json has failed for error: {:?}",e));

    unsafe {
        // call c++ dll
        let mut paw = best.get_piece_array_wrapper();

        // save the image
        (*paw).generate_test_image(format!("{}\\result.png",output_path).as_str());

        // save the binary
        (*paw).save_as_binary(format!("{}\\result.bin",output_path).as_str());

        // deallocate memory
        (*paw).destroy_piece_array_wrapper();
    }

    println!("Return 1");
    return true;
}