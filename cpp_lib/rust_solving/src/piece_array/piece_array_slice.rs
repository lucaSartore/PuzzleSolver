use crate::piece_array::PieceArray;
use crate::single_piece::{CSinglePiece, SingePiece};

/// allow to view a slice of a piece.
pub struct PieceArraySlice<'a>{
    original_piece_array: &'a PieceArray,
    offset_x: u64,
    offset_y: u64,
    size_x: u64,
    size_y: u64
}

impl PieceArraySlice<'_>{
    fn get_piece(&self, x: u64, y: u64) -> Result<&CSinglePiece,()>{
        if !(x < self.size_x || y < self.size_y){
            return Err(());
        }
        self.original_piece_array.get_piece(self.offset_x + x, self.offset_y + y)
    }
}

impl PartialEq for PieceArraySlice<'_> {
    fn eq(&self, other: &Self) -> bool {
        if self.size_y != other.size_y || self.size_x != other.size_x{
            return false;
        }
        for x in 0..self.size_x{
            for y in 0..self.size_y{
                if self.get_piece(x,y) != other.get_piece(x,y){
                    return false
                }
            }
        }
        return true;
    }
}

impl PieceArray {
    /// return a slice within the specified bounds
    /// x_max and y_max are INCLUDED
    pub fn get_slice(&self, x_min: u64, x_max: u64, y_min: u64, y_max: u64) ->PieceArraySlice {
        assert!(x_min<=x_max);
        assert!(y_min<=y_max);
        assert!(x_max<self.dim_x);
        assert!(y_max<self.dim_y);

        PieceArraySlice{
            original_piece_array: self,
            offset_x: x_min,
            offset_y: y_min,
            size_x: x_max-x_min+1,
            size_y: y_max-y_min+1
        }
    }
}