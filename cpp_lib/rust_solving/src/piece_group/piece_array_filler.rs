use crate::piece_array::PieceArray;
use crate::piece_group::{Comparable, HasKnownLevel, IsSubComponent, PieceGroup};
use crate::single_piece::SingePiece;


/// a trait for all single pieces, or group of pieces that can fill a piece array
pub trait PieceArrayFiller{
    /// given a mutable reference of a piece array to fill, this function fills it with
    /// grom the start x and y coordinates upward
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64);
}

impl<'a,T: PieceArrayFiller + Comparable + HasKnownLevel + Clone + IsSubComponent> PieceArrayFiller for PieceGroup<'a, T> {
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64){

        let or = self.orientation + recursive_orientation;

        self.get_top_left(recursive_orientation).fill_piece_array((to_fill),start_x,start_y,or);
        self.get_top_right(recursive_orientation).fill_piece_array((to_fill),start_x + T::SIDE_LEN,start_y,or);
        self.get_bottom_left(recursive_orientation).fill_piece_array((to_fill),start_x,start_y + T::SIDE_LEN,or);
        self.get_bottom_right(recursive_orientation).fill_piece_array((to_fill),start_x + T::SIDE_LEN,start_y + T::SIDE_LEN,or);
    }
}

impl PieceArrayFiller for SingePiece {
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64){
        if false{
            todo!("remove the unwraps")
        }

        let mut piece = self.clone();

        piece.rotate_by(recursive_orientation);

        to_fill.set_piece(start_x,start_y,piece.c_piece.clone()).unwrap();
    }
}