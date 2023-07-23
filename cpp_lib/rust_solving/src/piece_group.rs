use std::collections::HashSet;
use std::fmt::Debug;
use std::ops::DivAssign;
use crate::piece_array::PieceArray;
use crate::piece_group::Direction::LEFT;
use crate::piece_group::GroupCreationResult::{BottomLeftImpossibleCombination, BottomRightImpossibleFit};
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

pub trait PieceArrayFiller{
    /// given a mutable reference of a piece array to fill, this function fills it with
    /// grom the start x and y coordinates upward
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64);
}

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

/// can set the orientation of a piece or group of pieces
pub trait HasOrientation{
    fn set_orientation(&mut self, new_orientation: u64);
}

/// trait for all the elements that can be merged in one big component
pub trait IsSubComponent: Sized + Comparable{
    fn merge_together<'a>(top_left: &'a Self, top_right: &'a Self, bottom_right: &'a Self, bottom_left: &'a Self) -> GroupCreationResult<'a ,Self>;
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
#[derive(Debug)]
pub struct PieceGroup<'a,T: Comparable>{
    pub pieces: [&'a T;4],
    pub orientation: u64,
    pub ids: HashSet<u64>,
    pub shore: Shore
}

impl<T: Comparable> HasOrientation for PieceGroup<'_,T> {
    fn set_orientation(&mut self, new_orientation: u64){
        self.orientation = new_orientation
    }
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

impl<'a, T: Comparable> PieceGroup<'a, T> {

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

    pub fn new(top_left: &'a T, top_right: &'a T, bottom_right: &'a T, bottom_left: &'a T) -> GroupCreationResult<'a,T>{

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

        let ids = HashSet::new();

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
/// implementation of the new function for the first and above levels
impl<'a> PieceGroup<'a,SingePiece> {

    pub fn new(top_left: &'a SingePiece, top_right: &'a SingePiece, bottom_right: &'a SingePiece, bottom_left: &'a SingePiece) -> GroupCreationResult<'a,SingePiece>{

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
        let mut ret = PieceGroup::<'a,SingePiece>{
            pieces: [top_left, top_right,bottom_right, bottom_left],
            orientation: 0,
            ids,
            shore: Shore::new()
        };

        let shore_result = ret.calculate_and_set_shore();

        return GroupCreationResult::Ok(ret);
        todo!("remove the return");

        match shore_result {
            CalculateShoreResult::Ok(_) => {},
            CalculateShoreResult::BottomLeftImpossibleFit => return GroupCreationResult::BottomLeftImpossibleFit,
            CalculateShoreResult::BottomRightImpossibleFit => return GroupCreationResult::BottomRightImpossibleFit,
            CalculateShoreResult::TopRightImpossibleFit => return GroupCreationResult::TopRightImpossibleFit,
            CalculateShoreResult::AvregeIsTooLow => return GroupCreationResult::AvregeIsTooLow
        }

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
/*
impl<'a> PieceArrayFiller for PieceGroup<'a, SingePiece> {
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64){
        if false{
            todo!("remove the unwraps")
        }

        let or = self.orientation + recursive_orientation;

        let mut piece = *self.get_top_left(recursive_orientation);
        piece.rotate_by(or);

        to_fill.set_piece(start_x, start_y,piece).unwrap();

        piece = *self.get_top_right(recursive_orientation);
        print!("{:?}",piece);
        piece.rotate_by(or);
        print!("{:?}",piece);
        to_fill.set_piece(start_x+1,start_y,piece).unwrap();

        piece = *self.get_bottom_left(recursive_orientation);
        piece.rotate_by(or);
        to_fill.set_piece(start_x,start_y+1,piece).unwrap();

        piece = *self.get_bottom_right(recursive_orientation);
        piece.rotate_by(or);
        to_fill.set_piece(start_x+1,start_y+1,piece).unwrap();
    }
}
*/
impl<'a,T: PieceArrayFiller + Comparable + HasKnownLevel> PieceArrayFiller for PieceGroup<'a, T> {
    fn fill_piece_array(&self, to_fill: &mut PieceArray, start_x: u64, start_y: u64, recursive_orientation: u64){

        let or = self.orientation + recursive_orientation;

        self.get_top_left(recursive_orientation).fill_piece_array((to_fill),start_x,start_y,or);
        self.get_top_right(recursive_orientation).fill_piece_array((to_fill),start_x + T::SIDE_LEN,start_y,or);
        self.get_bottom_left(recursive_orientation).fill_piece_array((to_fill),start_x,start_y + T::SIDE_LEN,or);
        self.get_bottom_right(recursive_orientation).fill_piece_array((to_fill),start_x + T::SIDE_LEN,start_y + T::SIDE_LEN,or);
    }
}

impl<'a, T: HasKnownLevel + Comparable> HasKnownLevel for PieceGroup<'a, T>  {
    const LEVEL: u64 = 1 + T::LEVEL;
}


/// when trying to create a new group of piece many things can go wrong, this enum is the result
/// that represent all the errors, as well as the solution
#[derive(Debug)]
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

impl<'a, T: Comparable + Debug> GroupCreationResult<'a, T>{
    fn unwrap(self) -> PieceGroup<'a, T>{
        if let Self::Ok(e) = self{
            return e
        }
        panic!("call unwrap, bur value was: {:?}", self)
    }
}

impl IsSubComponent for SingePiece {
    fn merge_together<'a>(top_left: &'a Self, top_right: &'a Self, bottom_right: &'a Self, bottom_left: &'a Self) -> GroupCreationResult<'a, Self> {
        PieceGroup::<Self>::new(top_left, top_right, bottom_right, bottom_left)
    }
}

impl<'b,T: Comparable + HasSetInIt> IsSubComponent for PieceGroup<'b,T>{
    fn merge_together<'a>(top_left: &'a Self, top_right: &'a Self, bottom_right: &'a Self, bottom_left: &'a Self) -> GroupCreationResult<'a, Self> {
        PieceGroup::<Self>::new(top_left, top_right, bottom_right, bottom_left)
    }
}

/// when calculating a shore of a group of pieces, this enum will be return to
#[derive(Debug)]
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


mod tests{
    use crate::piece_array::{PieceArray, PieceArrayWrapper};
    use crate::piece_comparing::{Comparator, Initialized};
    use crate::piece_group::{PieceArrayFiller, PieceGroup};
    use crate::single_piece::SingePiece;

    #[test]
    fn test_fill_piece_array_2x2(){

        Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

        let mut pieces = vec![
            SingePiece::new(4,0),
            SingePiece::new(5,3),
            SingePiece::new(3,3),
            SingePiece::new(2,0),
        ];

        // remember: for this test is necessary to decidable the check on the new function. otherwise the unwrap will fail
        let mut pg = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],).unwrap();

        let mut pa = PieceArray::new(2,2);

        pg.fill_piece_array(&mut pa,0,0,0);

        unsafe {

            PieceArrayWrapper::load_images_to_piece_array_wrapper(r"..\..\dataset\test_2x3\divided");

            let paw = pa.get_piece_array_wrapper();

            (*paw).generate_test_image();

            (*paw).destroy_piece_array_wrapper();

        }
    }

    #[test]
    fn test_fill_piece_array_4x4() {


        Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

        let mut pieces = vec![
            SingePiece::new(4,0),
            SingePiece::new(5,3),
            SingePiece::new(3,3),
            SingePiece::new(2,0),
        ];
        // remember: for this test is necessary to decidable the check on the new function. otherwise the unwrap will fail
        let mut pg0 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],).unwrap();
        let mut pg1 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],).unwrap();
        let mut pg2 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],).unwrap();
        let mut pg3 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],).unwrap();

        pg1.set_orientation(3);
        pg2.set_orientation(2);
        pg3.set_orientation(1);

        let mut pg4x4 = PieceGroup::<PieceGroup::<SingePiece>>::new(&pg0,&pg1,&pg2,&pg3).unwrap();

        //pg4x4.set_orientation(1);

        let mut pa = PieceArray::new(4,4);

        pg4x4.fill_piece_array(&mut pa,0,0,0);

        unsafe {

            PieceArrayWrapper::load_images_to_piece_array_wrapper(r"..\..\dataset\test_2x3\divided");

            let paw = pa.get_piece_array_wrapper();

            (*paw).generate_test_image();

            (*paw).destroy_piece_array_wrapper();

        }
    }
}
