use crate::piece_group::{Comparable, IsSubComponent, PieceGroup};
use crate::single_piece::SingePiece;

pub trait HasKnownLevel{
    /// the level of the current struct
    /// level 1 is 2x2
    /// level 2 is 4x4
    /// level 3 is 8x8
    /// and so on
    const LEVEL: u64;
    /// the length of the side of the piece group
    const SIDE_LEN: u64 = (2 as u64).pow(Self::LEVEL as u32);
}
impl<'a, T: HasKnownLevel + Comparable + Clone + IsSubComponent> HasKnownLevel for PieceGroup<'a, T>  {
    const LEVEL: u64 = 1 + T::LEVEL;
}

impl HasKnownLevel for SingePiece{
    const LEVEL: u64 = 0;
}
