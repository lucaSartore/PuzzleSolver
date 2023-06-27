mod shore;

use shore::Shore;

pub enum Orientation{UP, RIGHT, DOWN, LEFT}

/// trait for all piece or group of pieces that can be compared between each other
pub trait Comparable{
    /// compare this piece two another piece from an orientation. the recursive orientations are needed to keep track of
    /// how many times the pieces has been rotated
    fn compare_to(&self, other: &Self, orientation: Orientation, this_recursive_rotation: i32, other_recursive_rotation: i32) -> Shore{
        return Shore::new();
    }
}


/// one single piece, keep the ID and his rotation
struct SingePiece{
    id: i32,
    rotation: i32
}

impl Comparable for SingePiece{
    fn compare_to(&self, other: &SingePiece, orientation: Orientation, this_recursive_rotation: i32, other_recursive_rotation: i32) -> Shore{
        return Shore::new();
    }
}

//struct PieceGroup<T: Comparable>{
struct PieceGroup<'a,T>{
    pub pieces: [&'a T;4],
    rotation: i32
}

impl<'a, T> PieceGroup<'a, T> {
    fn get_top_left(&self, orientation: i32)->&'a T{
        self.pieces[(0+self.rotation+orientation)%4]
    }
    fn get_top_right(&self, orientation: i32)->&'a T{
        self.pieces[(1+self.rotation+orientation)%4]
    }
    fn get_bottom_left(&self, orientation: i32)->&'a T{
        self.pieces[(2+self.rotation+orientation)%4]
    }
    fn get_bottom_right(&self, orientation: i32)->&'a T{
        self.pieces[(3+self.rotation+orientation)%4]
    }
}

impl<'a, T> Comparable for PieceGroup<'a, T> {
    fn compare_to(&self, other: &Self, orientation: Orientation, recursive_orientation: i32, recursive_orientation_other: i32) -> Shore{
        let mut s = Shore::new();
        match orientation {
            Orientation::RIGHT => {
                s += self
                    .get_top_right(recursive_orientation)
                    .compare_to(
                        orientation,
                        &other.get_top_left(recursive_orientation),
                        recursive_orientation + orientation,
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
                        &other.get_bottom_left(recursive_orientation),
                        recursive_orientation + orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
            Orientation::LEFT => {
                s += self
                    .get_top_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_right(recursive_orientation),
                        recursive_orientation + orientation,
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
                        &other.get_bottom_right(recursive_orientation),
                        recursive_orientation + orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
            Orientation::UP => {
                s += self
                    .get_top_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_left(recursive_orientation),
                        recursive_orientation + orientation,
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
                        &other.get_bottom_right(recursive_orientation),
                        recursive_orientation + orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
            Orientation::DOWN => {
                s += self
                    .get_bottom_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_left(recursive_orientation),
                        recursive_orientation + orientation,
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
                        &other.get_top_right(recursive_orientation),
                        recursive_orientation + orientation,
                        recursive_orientation_other + other.orientation,
                    );
            }
        }
        s
    }
}


fn main(){
    println!("hello world!");

    let pg = PieceGroup::<i32>{
        pieces: [&1,&2,&3,&4],
        rotation: 0
    };

}