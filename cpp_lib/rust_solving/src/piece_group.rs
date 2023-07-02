use std::collections::HashSet;
use crate::shore::Shore;
use crate::single_piece::SingePiece;
use crate::piece_comparing::{Comparator,Initialized,Uninitialized,InitializationResults};
#[derive(Clone,Copy)]
pub enum Direction {UP, RIGHT, DOWN, LEFT}

/// trait for all piece or group of pieces that can be compared between each other
pub trait Comparable{

    /// compare this piece two another piece from an orientation. the recursive orientations are needed to keep track of
    /// how many times the pieces has been rotated
    fn compare_to(&self, direction: Direction,other: &Self, recursive_orientation: u64, recursive_orientation_other: u64) -> Shore;
}

/// trait for all the level that has an hash set in ti (all except the first)
pub trait HasSetInIt{
    /// get a set containing the ids of the current set
    fn get_ids(&self) -> &HashSet<u64>;
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

pub struct PieceGroup<'a,T: Comparable>{
    pub pieces: [&'a T;4],
    pub orientation: u64,
    pub ids: HashSet<u64>,
    pub shore: Shore
}

impl<'a, T: Comparable> HasSetInIt for PieceGroup<'a, T> {
    fn get_ids(&self) -> &HashSet<u64> {
        &self.ids
    }
}

/// functions to get a specific sub piece of the group
impl<'a, T: Comparable> PieceGroup<'a, T> {
    fn get_top_left(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(0+self.orientation+recursive_orientation) as usize %4]
    }
    fn get_top_right(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(1+self.orientation+recursive_orientation) as usize %4]
    }
    fn get_bottom_left(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(2+self.orientation+recursive_orientation) as usize %4]
    }
    fn get_bottom_right(&self, recursive_orientation: u64)->&'a T{
        self.pieces[(3+self.orientation+recursive_orientation) as usize %4]
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

impl<'a, T: Comparable> PieceGroup<'a, T> {

    /// calculate the shore of the current group of pieces
    fn calculate_shore(&self) -> CalculateShoreResult{
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

        // eventing was ok, so i can return the shore itself
        return CalculateShoreResult::Ok(shore);
    }

}

impl<'a, T: Comparable> PieceGroup<'a, PieceGroup<'_,T>> {
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
impl<'a,T: Comparable + HasSetInIt> PieceGroup<'a,T> {

    fn new(top_left: &'a T, top_right: &'a T, bottom_right: &'a T, bottom_left: &'a T) -> GroupCreationResult<'a,T>{

        let top_left_ids = top_left.get_ids();
        let top_right_ids = top_right.get_ids();

        //if the two pieces has an element in common the group is invalid
        if !top_left_ids.is_disjoint(top_right_ids) {
            return GroupCreationResult::TopRightImpossibleCombination;
        }

        // create the set of the group we are creating
        let mut ids = top_left_ids.clone();
        ids.extend(top_right_ids);

        let bottom_right_ids = bottom_right.get_ids();

        //if the two pieces has an element in common the group is invalid
        if !ids.is_disjoint(bottom_right_ids) {
            return GroupCreationResult::BottomRightImpossibleCombination;
        }

        //create the set of the group we are creating
        ids.extend(top_right_ids);

        let bottom_left_ids = bottom_left.get_ids();

        //if the two pieces has an element in common the group is invalid
        if !ids.is_disjoint(bottom_left_ids) {
            return GroupCreationResult::BottomLeftImpossibleCombination;
        }

        //create the set of the group we are creating
        ids.extend(bottom_left_ids);

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
/// implementation of the new function for the first and above levels
impl<'a> PieceGroup<'a,SingePiece> {

    fn new(top_left: &'a SingePiece, top_right: &'a SingePiece, bottom_right: &'a SingePiece, bottom_left: &'a SingePiece) -> GroupCreationResult<'a,SingePiece>{

        //if they have the same id, return an error
        if top_left.get_id() == top_right.get_id(){
            return GroupCreationResult::TopRightImpossibleCombination;
        }

        //if they have the same id, return an error
        if top_left.get_id() == bottom_right.get_id() ||  top_right.get_id() == bottom_right.get_id(){
            return GroupCreationResult::BottomRightImpossibleCombination;
        }

        //if they have the same id, return an error
        if top_left.get_id() == bottom_left.get_id() ||  top_right.get_id() == bottom_left.get_id() || bottom_right.get_id() == bottom_left.get_id(){
            return GroupCreationResult::BottomRightImpossibleCombination;
        }

        let ids = HashSet::from(
            [
                top_left.get_id(),
                top_right.get_id(),
                bottom_right.get_id(),
                bottom_left.get_id()
            ]
        );

        // create the return object
        let ret = PieceGroup::<'a,SingePiece>{
            pieces: [top_left, top_right,bottom_right, bottom_left],
            orientation: 0,
            ids,
            shore: Shore::new()
        };

        todo!();// calculate the shore

        return GroupCreationResult::Ok(ret);
    }

}


impl<'a, T:Comparable> Comparable for PieceGroup<'a, T> {
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


/// when trying to create a new group of piece many things can go wrong, this enum is the result
/// that represent all the errors, as well as the solution
pub enum GroupCreationResult<'a, T: Comparable>{
    /// created successfully, and return the type
    Ok(PieceGroup<'a, T>),
    /// the top right piece contain a piece that is also contained in one of the previous pieces
    TopRightImpossibleCombination,
    /// the bottom right piece contain a piece that is also contained in one of the previous pieces
    BottomRightImpossibleCombination,
    /// the bottom left piece contain a piece that is also contained in one of the previous pieces
    BottomLeftImpossibleCombination,
    /// the top right piece dose not fit with the preius piece
    TopRightImpossibleFit,
    /// the bottom right piece dose not fit with the preius piece
    BottomRightImpossibleFit,
    /// the bottom left piece dose not fit with the preius piece
    BottomLeftImpossibleFit,
    /// the shore of the piece is lower that the minimum threshold
    AvregeIsTooLow
}

/// when calculating a shore of a group of pieces, this enum will be return to
pub enum CalculateShoreResult{
    /// all the pieces where compatible, and the shore has been calculated
    Ok(Shore),
    /// the top right piece dose not fit with the preius piece
    TopRightImpossibleFit,
    /// the bottom right piece dose not fit with the preius piece
    BottomRightImpossibleFit,
    /// the bottom left piece dose not fit with the preius piece
    BottomLeftImpossibleFit,
    /// the shore of the piece is lower that the minimum threshold
    AvregeIsTooLow
}