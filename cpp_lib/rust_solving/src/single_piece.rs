use piece_group::Comparable;
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