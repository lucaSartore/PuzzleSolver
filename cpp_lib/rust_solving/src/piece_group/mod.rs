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
use crate::piece_group_holder::PieceRef;

mod comparable;
pub use comparable::Comparable;

mod intersection_comparable;

mod direction;
pub use direction::Direction;

mod piece_array_filler;
pub use piece_array_filler::PieceArrayFiller;

mod has_known_level;
pub use has_known_level::HasKnownLevel;

mod has_orientation;
pub use has_orientation::HasOrientation;

mod has_basic_components;
pub use has_basic_components::HasBasicComponents;

mod is_sub_component;
pub use is_sub_component::IsSubComponent;

mod return_enums;
pub use  return_enums::{CalculateShoreResult,GroupCreationResult};

mod next_level_or_panic;
pub use next_level_or_panic::NextLevelOrPanic;

mod add_shore_of_sub_components;
pub use add_shore_of_sub_components::AddShoreOfSubComponents;
use crate::constants::MIN_SHORE_PIECE_GROUP;
use crate::piece_basics_components::PieceBasicComponents;

mod test;


#[derive(Debug, Clone)]
pub struct PieceGroup<'a,T: Comparable + Clone + IsSubComponent>{
    pub pieces: [&'a T;4],
    pub orientation: u64,
    pub basic_components: PieceBasicComponents,
    pub shore: Shore
}

impl<'a, T: Comparable + Clone + IsSubComponent> PieceGroup<'a, T> {
    fn get_basic_components(&self) -> &PieceBasicComponents {
        &self.basic_components
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
/// `already_calculated_shores` is a vect with the shores for the comparison top_left-top_right; top_right-bottom_right and bottom_right-bottom_left
impl<'a,T: Comparable + Clone + IsSubComponent + HasBasicComponents<T> + AddShoreOfSubComponents> PieceGroup<'a,T> {

    pub fn new(top_left: &'a T, top_right: &PieceRef<'a,T>, bottom_right: &PieceRef<'a,T>, bottom_left:  &PieceRef<'a,T>) -> GroupCreationResult<'a,T>{

        let basic_components = T::merge_basic_components(top_left, top_right, bottom_right, bottom_left);

        let basic_components = match basic_components {
            Result::Ok(e) => e,
            Result::Err(err) => return err
        };

        // comparing top border
        let mut shore = bottom_left.reference.compare_to(Direction::UP,top_left,0,0);
        // returning error if the piece is impossible
        if shore.get_shore() == 0 {
            return BottomLeftImpossibleFit;
        }
        shore += top_right.shore + bottom_right.shore + bottom_left.shore;

        // returning an error if the piece is impossible
        if shore.get_shore() <= MIN_SHORE_PIECE_GROUP{
            return AvregeIsTooLow;
        }

        // adding the avrege of the 4 sub components
        top_left.add_shore_of_sub_components(&mut shore);
        top_right.reference.add_shore_of_sub_components(&mut shore);
        bottom_right.reference.add_shore_of_sub_components(&mut shore);
        bottom_left.reference.add_shore_of_sub_components(&mut shore);

        // create the return object
        let ret = Self{
            pieces: [top_left, top_right.reference,bottom_right.reference, bottom_left.reference],
            orientation: 0,
            basic_components,
            shore
        };

        GroupCreationResult::Ok(ret)
    }

}