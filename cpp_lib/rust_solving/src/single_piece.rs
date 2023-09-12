use std::collections::HashSet;
use crate::piece_array::PieceArray;
use crate::piece_group::{HasKnownLevel, PieceArrayFiller};
use crate::piece_group::{Comparable, HasOrientation, PieceGroup};
use crate::piece_comparing::{Comparator, Initialized, Uninitialized, InitializationResults};
use crate::piece_basics_components::PieceBasicComponents;

#[repr(C)]
#[derive(Debug)]
#[derive(Clone)]
pub struct CSinglePiece{
    pub id: u64,
    pub orientation: u64
}

impl CSinglePiece{
    pub fn new(id: u64, orientation: u64) -> Self{
        Self{
            id,orientation
        }
    }
}

/// one single piece, keep the ID and his rotation
#[derive(Debug)]
#[repr(C)]
#[derive(Clone)]
pub struct SingePiece{
    /// representation of this piece in a way that C can understand
    pub c_piece: CSinglePiece,
    pub basic_components: PieceBasicComponents,
    /// zero sized type to compare the pieces, dose not need to be passed to c
    pub comparator: Comparator<Initialized>
}

impl SingePiece {
    pub fn get_id(&self) -> u64{
        self.c_piece.id
    }
    pub fn get_orientation(&self) -> u64{
        self.c_piece.orientation
    }
    pub fn new(id: u64, orientation: u64) -> Self{
        assert!(orientation < 4);
        if let Some(comparator) = Comparator::<Initialized>::get_initialized_comparator(){
            return Self{
                c_piece: CSinglePiece{id,orientation},
                comparator,
                basic_components: PieceBasicComponents::new(id as usize)
            };
        }else{
            panic!("impossible to create a single unless you initialize the comparator first");
        }
    }

    pub fn rotate_by(&mut self, q: u64){
        self.c_piece.orientation = (self.c_piece.orientation+q)%4;
    }
}





