use crate::piece_group::{CanCreateSet, Comparable, GroupCreationResult, PieceGroup};
use crate::single_piece::SingePiece;

/// trait for all the elements that can be merged in one big component
pub trait IsSubComponent: Sized + Comparable + Clone{
    fn merge_together<'a>(top_left: &'a Self, top_right: &'a Self, bottom_right: &'a Self, bottom_left: &'a Self) -> GroupCreationResult<'a ,Self>;
}

impl IsSubComponent for SingePiece {
    fn merge_together<'a>(top_left: &'a Self, top_right: &'a Self, bottom_right: &'a Self, bottom_left: &'a Self) -> GroupCreationResult<'a, Self> {
        PieceGroup::<Self>::new(top_left, top_right, bottom_right, bottom_left)
    }
}

impl<'b,T: Comparable + Clone + IsSubComponent + CanCreateSet<T>> IsSubComponent for PieceGroup<'b,T>{
    fn merge_together<'a>(top_left: &'a Self, top_right: &'a Self, bottom_right: &'a Self, bottom_left: &'a Self) -> GroupCreationResult<'a, Self> {
        PieceGroup::<Self>::new(top_left, top_right, bottom_right, bottom_left)
    }
}