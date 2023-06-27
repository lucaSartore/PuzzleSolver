mod shore;

use Direction::DOWN;
use shore::Shore;

#[derive(Clone,Copy)]
pub enum Direction {UP, RIGHT, DOWN, LEFT}

/// trait for all piece or group of pieces that can be compared between each other
pub trait Comparable{
    /// compare this piece two another piece from an orientation. the recursive orientations are needed to keep track of
    /// how many times the pieces has been rotated
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: usize, recursive_orientation_other: usize) -> Shore{
        return Shore::new();
    }
}


/// one single piece, keep the ID and his rotation
struct SingePiece{
    id: usize,
    orientation: usize
}

impl Comparable for SingePiece{
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: usize, recursive_orientation_other: usize) -> Shore{
        return Shore::new();
    }
}

struct PieceGroup<'a,T: Comparable>{
//struct PieceGroup<'a,T>{
    pub pieces: [&'a T;4],
    orientation: usize
}

/// when trying to create a new group of piece many things can go wrong, this enum is the result
/// that represent all the errors, as well as the solution
pub enum GroupCreationResult<'a, T: Comparable>{
    Ok(PieceGroup<'a, T>),
    TopRightImpossibleFit,
    RopLeftImpossibleFit,
    BottomLeftImpossibleFit,
    BottomRightImpossibleFit

}

/// functions to get a specific sub piece of the group
impl<'a, T: Comparable> PieceGroup<'a, T> {
    fn get_top_left(&self, recursive_orientation: usize)->&'a T{
        self.pieces[(0+self.orientation+recursive_orientation)%4]
    }
    fn get_top_right(&self, recursive_orientation: usize)->&'a T{
        self.pieces[(1+self.orientation+recursive_orientation)%4]
    }
    fn get_bottom_left(&self, recursive_orientation: usize)->&'a T{
        self.pieces[(2+self.orientation+recursive_orientation)%4]
    }
    fn get_bottom_right(&self, recursive_orientation: usize)->&'a T{
        self.pieces[(3+self.orientation+recursive_orientation)%4]
    }

    fn set_top_left(&mut self, to_set: &'a T){
        self.pieces[(0+self.orientation)%4] = to_set;
    }
    fn set_top_right(&mut self, to_set: &'a T){
        self.pieces[(1+self.orientation)%4] = to_set;
    }
    fn set_bottom_left(&mut self, to_set: &'a T){
        self.pieces[(2+self.orientation)%4] = to_set;
    }
    fn set_bottom_right(&mut self, to_set: &'a T){
        self.pieces[(3+self.orientation)%4] = to_set;
    }

    /// create a new group of pieces with the 4 sub pieces
    fn new<'b>(top_left: &'b T, top_right: &'b T, bottom_right: &'b T, bottom_left: &'b T) -> PieceGroup<'b,T>{
        // create the object itself
        let n = PieceGroup::<'b,T>{
            pieces: [top_left,top_right,bottom_left,bottom_left],
            orientation: 0
        };


        n
    }

}


impl<'a, T:Comparable> Comparable for PieceGroup<'a, T> {
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: usize, recursive_orientation_other: usize) -> Shore{
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
                s.add_assign(&self
                    .get_bottom_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_left(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    )
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
                s.add_assign(&self
                    .get_bottom_left(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    )
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
                s.add_assign(&self
                    .get_top_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_bottom_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    )
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
                s.add_assign(&self
                    .get_bottom_right(recursive_orientation)
                    .compare_to(
                        direction,
                        &other.get_top_right(recursive_orientation_other),
                        recursive_orientation + self.orientation,
                        recursive_orientation_other + other.orientation,
                    )
                );
            }
        }
        s
    }
}


impl Comparable for i32 {
    fn compare_to(&self, direction: Direction, other: &Self, recursive_orientation: usize, recursive_orientation_other: usize) -> Shore {
        todo!()
    }
}

fn main(){
    println!("hello world!");

    let pg = PieceGroup::<i32>{
        pieces: [&1,&2,&3,&4],
        orientation: 0
    };

    pg.compare_to(DOWN,&pg,0,0);

}