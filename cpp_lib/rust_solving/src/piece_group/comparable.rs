use crate::piece_group::Direction;
use crate::shore::Shore;
use crate::single_piece::SingePiece;

/// trait for all piece or group of pieces that can be compared between each other
pub trait Comparable{
    /// compare this piece two another piece from an orientation. the recursive orientations are needed to keep track of
    /// how many times the pieces has been rotated
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: u64, recursive_orientation_other: u64) -> Shore;
}
impl Comparable for SingePiece{
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: u64, recursive_orientation_other: u64) -> Shore{
        let v = self.comparator.compare(
            self.get_id(),
            other.get_id(),
            self.get_orientation(),
            other.get_orientation()
        );
        return Shore::with_shore(v);
    }

}