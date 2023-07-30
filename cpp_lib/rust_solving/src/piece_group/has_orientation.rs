use crate::piece_group::{Comparable, IsSubComponent, PieceGroup};
use crate::single_piece::SingePiece;

/// can set the orientation of a piece or group of pieces
pub trait HasOrientation{
    fn set_orientation(&mut self, new_orientation: u64);
}

impl<T: Comparable + Clone + IsSubComponent> HasOrientation for PieceGroup<'_,T> {
    fn set_orientation(&mut self, new_orientation: u64){
        self.orientation = new_orientation
    }
}

impl HasOrientation for SingePiece {
    fn set_orientation(&mut self, new_orientation: u64){
        self.orientation = new_orientation
    }
}