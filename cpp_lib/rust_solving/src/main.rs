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

use std::collections::HashSet;
use piece_group::PieceGroup;
use piece_group::Comparable;
use piece_group::Direction;
use shore::Shore;


impl Comparable for i32 {
    fn compare_to(&self, _: Direction, _: &Self, _: u64, _: u64) -> Shore {
        todo!()
    }
}


fn main(){
    println!("hello world!");

    let pg = PieceGroup::<i32>{
        pieces: [&1,&2,&3,&4],
        orientation: 0,
        ids: HashSet::default(),
        shore: Shore::new()
    };

    pg.compare_to(Direction::DOWN,&pg,0,0);

}