use crate::piece_group::{Comparable, IsSubComponent, PieceGroup};
use crate::shore::Shore;
use crate::single_piece::SingePiece;

/// a piece group may also consider all
pub trait AddShoreOfSubComponents{
    fn add_shore_of_sub_components(&self,add_to: &mut Shore);
}

// single piece has no sub components
impl AddShoreOfSubComponents for SingePiece{
    fn add_shore_of_sub_components(&self, add_to: &mut Shore) {}
}

impl<T: Comparable + Clone + IsSubComponent> AddShoreOfSubComponents for PieceGroup<'_,T>{
    fn add_shore_of_sub_components(&self, add_to: &mut Shore) {
        (*add_to) += self.shore;
    }
}