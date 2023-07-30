use std::collections::HashSet;
use std::fmt::Debug;
use std::ops::DivAssign;
use std::process::id;
use crate::piece_array::PieceArray;
use crate::piece_group::Direction::LEFT;
use crate::piece_group::GroupCreationResult::{BottomLeftImpossibleCombination, BottomRightImpossibleFit};
use crate::shore::Shore;
use crate::single_piece::SingePiece;
use crate::piece_comparing::{Comparator,Initialized,Uninitialized,InitializationResults};

mod comparable;
pub use comparable::Comparable;

mod direction;
pub use direction::Direction;

mod piece_array_filler;
pub use piece_array_filler::PieceArrayFiller;

mod has_known_level;
pub use has_known_level::HasKnownLevel;

mod has_orientation;
pub use has_orientation::HasOrientation;

mod can_create_set;
pub use can_create_set::CanCreateSet;

mod is_sub_component;
pub use is_sub_component::IsSubComponent;

mod return_enums;
pub use  return_enums::{CalculateShoreResult,GroupCreationResult};

mod test;


#[derive(Debug, Clone)]
pub struct PieceGroup<'a,T: Comparable + Clone + IsSubComponent>{
    pub pieces: [&'a T;4],
    pub orientation: u64,
    pub ids: HashSet<u64>,
    pub shore: Shore
}

impl<'a, T: Comparable + Clone + IsSubComponent> PieceGroup<'a, T> {
    fn get_ids(&self) -> &HashSet<u64> {
        &self.ids
    }
}

/// functions to get a specific sub piece of the group
impl<'a, T: Comparable + Clone + IsSubComponent> PieceGroup<'a, T> {
    fn get_top_left(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(0+self.orientation+recursive_orientation) as usize %4]
    }
    fn get_top_right(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(1+self.orientation+recursive_orientation) as usize %4]
    }
    fn get_bottom_left(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(3+self.orientation+recursive_orientation) as usize %4]
    }
    fn get_bottom_right(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(2+self.orientation+recursive_orientation) as usize %4]
    }

    fn set_top_left(&mut self, to_set: &'a T){
        self.pieces[(0+self.orientation)  as usize %4] = to_set;
    }
    fn set_top_right(&mut self, to_set: &'a T){
        self.pieces[(1+self.orientation) as usize %4] = to_set;
    }
    fn set_bottom_left(&mut self, to_set: &'a T){
        self.pieces[(2+self.orientation) as usize %4] = to_set;
    }
    fn set_bottom_right(&mut self, to_set: &'a T){
        self.pieces[(3+self.orientation) as usize %4] = to_set;
    }
}

impl<'a, T: Comparable + Clone + IsSubComponent> PieceGroup<'a, T> {

    /// calculate the shore of the current group of pieces
    fn calculate_and_set_shore(&mut self) -> CalculateShoreResult{
        let mut shore = Shore::new();

        // comparing top border
        shore += self.get_top_left(0).compare_to(Direction::RIGHT,self.get_top_right(0),self.orientation,self.orientation);
        // trowing an error if the piece is impossible
        if shore.get_shore() == 0 {
            return CalculateShoreResult::TopRightImpossibleFit;
        }


        // comparing right border
        shore += self.get_top_right(0).compare_to(Direction::DOWN,self.get_bottom_right(0),self.orientation,self.orientation);
        // trowing an error if the piece is impossible
        if shore.get_shore() == 0 {
            return  CalculateShoreResult::BottomRightImpossibleFit;
        }


        // comparing bottom border
        shore += self.get_bottom_right(0).compare_to(Direction::LEFT,self.get_bottom_left(0),self.orientation,self.orientation);
        // trowing an error if the piece is impossible
        if shore.get_shore() == 0 {
            return CalculateShoreResult::BottomLeftImpossibleFit;
        }

        // comparing bottom border
        shore += self.get_bottom_left(0).compare_to(Direction::UP,self.get_top_left(0),self.orientation,self.orientation);
        // trowing an error if the piece is impossible
        if shore.get_shore() == 0 {
            return CalculateShoreResult::BottomLeftImpossibleFit;
        }

        // trowing an error if the piece is impossible
        if shore.get_shore() <= 100 {
            todo!("insert constant and return code");
            return CalculateShoreResult::AvregeIsTooLow;
        }

        // set the calculated shore
        self.shore = shore;

        // eventing was ok, so i can return the shore itself
        return CalculateShoreResult::Ok(shore);
    }

}

impl<'a, T: Comparable + Clone + IsSubComponent  + CanCreateSet<T>> PieceGroup<'a, PieceGroup<'_,T>> {
    /// calculate the sore of the sub components that make up this piece
    /// this implementation is for all group of level 2 or higher
    fn calculate_internal_shore(&self) -> Shore{

        let mut shore = self.get_top_right(0).shore;
        shore += self.get_top_left(0).shore;
        shore += self.get_bottom_left(0).shore;
        shore += self.get_bottom_right(0).shore;

        return shore;
    }
}
impl<'a> PieceGroup<'a, SingePiece> {
    /// calculate the sore of the sub components that make up this piece
    /// this implementation is for group of level 1
    fn calculate_internal_shore(&self) -> Shore{
        return Shore::new();
    }
}

/// implementation of the new function for the second and above levels
impl<'a,T: Comparable + Clone + IsSubComponent + CanCreateSet<T>> PieceGroup<'a,T> {

    pub fn new(top_left: &'a T, top_right: &'a T, bottom_right: &'a T, bottom_left: &'a T) -> GroupCreationResult<'a,T>{
        let ids = T::get_set(&top_left, &top_right, &bottom_right, &bottom_left);

        let ids = match ids {
            Result::Ok(e) => e,
            Result::Err(err) => return err
        };

        if false{
            todo!("calculate shore")
        }

        // create the return object
        let ret = Self{
            pieces: [top_left, top_right,bottom_right, bottom_left],
            orientation: 0,
            ids,
            shore: Shore::new()
        };

        GroupCreationResult::Ok(ret)
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
