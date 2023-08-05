
extern crate libc;
extern crate core;

#[allow(dead_code)]
mod shore;
#[allow(dead_code)]
mod piece_array;
#[allow(dead_code)]
mod single_piece;
#[allow(dead_code)]
mod piece_group;
#[allow(dead_code)]
mod piece_comparing;
#[allow(dead_code)]
mod piece_group_holder;
#[allow(dead_code)]
mod solver;

mod constants;

use std::collections::HashSet;
use piece_array::PieceArrayWrapper;
use piece_comparing::{Comparator, InitializationResults, Initialized};
use piece_group::PieceGroup;
use piece_group::Comparable;
use piece_group::Direction;
use piece_group_holder::PieceGroupHolder;
use shore::Shore;
use single_piece::SingePiece;
use crate::constants::MIN_SHORE_PIECE_ARRAY;
use crate::piece_array::PieceArray;
use crate::piece_group::PieceArrayFiller;
use crate::solver::TEST;


fn main(){
    // load data for the comparator
    let init_cmp = Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_4x4\connections").unwrap();



    unsafe {
        PieceArrayWrapper::load_images_to_piece_array_wrapper(r"..\..\dataset\test_4x4\divided");
    }


    // create the vector with the basic pieces inside
    let number_of_pieces = init_cmp.get_number_of_pieces();
    let mut v = Vec::<SingePiece>::with_capacity(number_of_pieces);
    for n in 0..init_cmp.get_number_of_pieces(){
        v.push(
            SingePiece::new(n as u64, 0)
        );
    }


    // convert the vector in piece group holder
    let pgh = PieceGroupHolder::new(v);


    /*
    //4_5_6_12_3_3_2_0
    let tl = pgh.get(4,3);

    let tr = pgh.get(5,3);

    let br = pgh.get(6,2);

    let bl = pgh.get(12,0);

    let cmp = piece_comparing::Comparator::<Initialized>::get_initialized_comparator().unwrap();
    println!("{:?}",cmp.compare(3,9,2,1));

    let pg = PieceGroup::new(tl,tr,br,bl);

    let pg = pg.unwrap();

    // create the piece array with the appropriate size
    let mut pa = PieceArray::new(2,2);

    // fill the piece array with the piece group
    pg.fill_piece_array(&mut pa,0,0,0);

    // calling the c++ dll
    unsafe {
        let mut paw = pa.get_piece_array_wrapper();

        (*paw).generate_test_image("missing.png");

        println!("shore: {}",(*paw).get_shore());

        // deallocate memory
        (*paw).destroy_piece_array_wrapper();
    }
    println!("{:?}",pg);
*/


    solver::solve(&pgh);

}