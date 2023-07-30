use crate::piece_group::{Direction, IsSubComponent, PieceGroup};
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

        let side_this = (self.orientation + (direction as u64))%4;

        let side_other = (other.orientation + (direction as u64) + 2)%4;

        let v = self.comparator.compare(
            self.get_id(),
            other.get_id(),
            side_this,
            side_other
        );
        return Shore::with_shore(v);
    }
}


impl<'a, T:Comparable + Clone + IsSubComponent> Comparable for PieceGroup<'a, T> {
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: u64, recursive_orientation_other: u64) -> Shore{
        let mut s;
        match direction {
            Direction::RIGHT => {
                s = self.get_top_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_left(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
                // Early evaluation to save time.
                if s.get_shore() == 0 {
                    return s;
                }
                s += self
                    .get_bottom_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_left(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
            Direction::LEFT => {
                s = self
                    .get_top_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
                // Early evaluation to save time.
                if s.get_shore() == 0 {
                    return s;
                }
                s += self
                    .get_bottom_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
            Direction::UP => {
                s = self
                    .get_top_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_left(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
                // Early evaluation to save time.
                if s.get_shore() == 0 {
                    return s;
                }
                s += self
                    .get_top_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
            Direction::DOWN => {
                s = self
                    .get_bottom_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_left(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
                // Early evaluation to save time.
                if s.get_shore() == 0 {
                    return s;
                }
                s += self
                    .get_bottom_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
        }
        s
    }
}
