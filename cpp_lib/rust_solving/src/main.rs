
extern crate libc;

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


use std::collections::HashSet;
use piece_array::PieceArrayWrapper;
use piece_comparing::{Comparator, InitializationResults, Initialized};
use piece_group::PieceGroup;
use piece_group::Comparable;
use piece_group::Direction;
use piece_group_holder::PieceGroupHolder;
use shore::Shore;
use single_piece::SingePiece;


impl Comparable for i32 {
    fn compare_to(&self, _: Direction, _: &Self, _: u64, _: u64) -> Shore {
        todo!()
    }
}


fn main(){
    // load data for the comparator
    let init_cmp = Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

    // check for the result of the initialization
    let init_cmp = match init_cmp {
        InitializationResults::Ok(e) => e,
        _ => {
            println!("initialization failed because of error: {:?}",init_cmp);
            return;
        }
    };

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

    solver::solve(&pgh);

}