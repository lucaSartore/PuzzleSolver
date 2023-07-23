use std::collections::HashSet;
use crate::piece_array::PieceArray;
use crate::piece_group::{HasKnownLevel, HasSetInIt, PieceArrayFiller};
use crate::piece_group::{Comparable, HasOrientation, PieceGroup};
use crate::piece_comparing::{Comparator, Initialized, Uninitialized, InitializationResults};

/// one single piece, keep the ID and his rotation
#[derive(Debug)]
#[repr(C)]
#[allow(improper_ctypes)]
#[derive(Copy,Clone)]
pub struct SingePiece{
    id: u64,
    orientation: u64,
    /// zero sized type to compare the pieces, dose not need to be passed to c
    pub comparator: Comparator<Initialized>
}

impl SingePiece {
    pub fn get_id(&self) -> u64{
        self.id
    }
    pub fn get_orientation(&self) -> u64{
        self.orientation
    }
    pub fn new(id: u64, orientation: u64) -> Self{
        assert!(orientation < 4);
        if let Some(comparator) = Comparator::<Initialized>::get_initialized_comparator(){
            return Self{
                id,
                orientation,
                comparator
            };
        }else{
            panic!("impossible to create a single unless you initialize the comparator first");
        }
    }

    pub fn rotate_by(&mut self, q: u64){
        self.orientation = (self.orientation+q)%4;
    }
}

impl HasOrientation for SingePiece {
    fn set_orientation(&mut self, new_orientation: u64){
        self.orientation = new_orientation
    }
}

impl HasKnownLevel for SingePiece{
    const LEVEL: u64 = 0;
}


impl PieceArrayFiller for SingePiece {
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64){
        if false{
            todo!("remove the unwraps")
        }

        let mut piece = self.clone();

        piece.rotate_by(recursive_orientation);

        to_fill.set_piece(start_x,start_y,piece).unwrap();
    }
}