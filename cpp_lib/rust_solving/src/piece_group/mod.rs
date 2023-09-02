use std::collections::HashSet;
use std::fmt::Debug;
use std::ops::DivAssign;
use std::process::id;
use crate::piece_array::PieceArray;
use crate::piece_group::Direction::LEFT;
use crate::piece_group::GroupCreationResult::{AvregeIsTooLow, BottomLeftImpossibleCombination, BottomLeftImpossibleFit, BottomRightImpossibleFit, TopRightImpossibleFit};
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

mod next_level_or_panic;
pub use next_level_or_panic::NextLevelOrPanic;

mod add_shore_of_sub_components;
pub use add_shore_of_sub_components::AddShoreOfSubComponents;
use crate::constants::MIN_SHORE_PIECE_GROUP;

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


impl<'a> PieceGroup<'a, SingePiece> {
    /// calculate the sore of the sub components that make up this piece
    /// this implementation is for group of level 1
    fn calculate_internal_shore(&self) -> Shore{
        return Shore::new();
    }
}

/// implementation of the new function for the second and above levels
impl<'a,T: Comparable + Clone + IsSubComponent + CanCreateSet<T> + AddShoreOfSubComponents> PieceGroup<'a,T> {

    pub fn new(top_left: &'a T, top_right: &'a T, bottom_right: &'a T, bottom_left: &'a T) -> GroupCreationResult<'a,T>{
        let ids = T::get_set(&top_left, &top_right, &bottom_right, &bottom_left);

        let ids = match ids {
            Result::Ok(e) => e,
            Result::Err(err) => return err
        };

        let mut shore_tot;
        let mut shore;

        // comparing top border
        shore = top_left.compare_to(Direction::RIGHT,top_right,0,0);
        // returning error if the piece is impossible
        if shore.get_shore() == 0 {
            return TopRightImpossibleFit;
        }
        shore_tot = shore;

        // comparing top border
        shore = top_right.compare_to(Direction::DOWN,bottom_right,0,0);
        // returning error if the piece is impossible
        if shore.get_shore() == 0  {
            return BottomRightImpossibleFit;
        }
        shore_tot += shore;


        // comparing top border
        shore = bottom_right.compare_to(Direction::LEFT,bottom_left,0,0);
        // returning error if the piece is impossible
        if shore.get_shore() == 0 {
            return BottomLeftImpossibleFit;
        }
        shore_tot += shore;


        // comparing top border
        shore = bottom_left.compare_to(Direction::UP,top_left,0,0);
        // returning error if the piece is impossible
        if shore.get_shore() == 0 {
            return BottomLeftImpossibleFit;
        }
        shore_tot += shore;


        // returning an error if the piece is impossible
        if shore_tot.get_shore() <= MIN_SHORE_PIECE_GROUP{
            //todo!(uncommeent this)
            return AvregeIsTooLow;
        }

        // adding the avrege of the 4 sub components
        top_left.add_shore_of_sub_components(&mut shore_tot);
        top_right.add_shore_of_sub_components(&mut shore_tot);
        bottom_right.add_shore_of_sub_components(&mut shore_tot);
        bottom_left.add_shore_of_sub_components(&mut shore_tot);


        // create the return object
        let ret = Self{
            pieces: [top_left, top_right,bottom_right, bottom_left],
            orientation: 0,
            ids,
            shore: shore_tot
        };

        GroupCreationResult::Ok(ret)
    }

}