use std::cell::RefCell;
use std::fmt::{Debug, Formatter};
use std::iter::zip;
use std::sync::Mutex;
use std::time::Duration;
use libc::tolower;
use rayon::prelude::*;
use crate::piece_group::{Comparable, Direction, HasOrientation};
use crate::shore::Shore;


/// this struct is used to keep all of the possible combination of one rotation of piece array group in scope
pub struct PieceGroupHolder<T: Clone + HasOrientation + Send + Sync + Comparable>{
    /// the 4 vec represent the 4 orientation, each element represent a specific piece
    pieces: [Vec<T>;4],
    size: usize,
}
impl<'a,T: Clone + HasOrientation + Send + Sync + Comparable> PieceGroupHolder<T>{
    /// get a vector with the pieces inside, and create all the possible orientations of ir
    pub fn new(vec: Vec<T>) -> Self {
        let size = vec.len();

        let mut pieces = [vec.clone(), vec.clone(), vec.clone(), vec];

        for i in 1..4 {
            pieces[i].par_iter_mut().for_each(|x| x.set_orientation(i as u64));
        }

        let mut to_return = Self {
            pieces,
            size,
        };

        to_return
    }
    pub fn get(&self, id: usize, orientation: usize) -> &T{
        &(self.pieces[orientation][id])
    }

    pub fn get_size(&self)->usize{
        return self.size;
    }

    // return a struct containing the pieces that match divided by side. and id
    /*
    fn get_matches(&self) -> MatchForGroupPieceHolder<T>{

        // create array of pieces that can mathc to speed up testing combination process
        let mut matching = Vec::with_capacity(self.size);
        for i in 0..self.size{
            matching.push(PieceThatMatchForEverySide::<T>::new());
        }

        let matching_mutex = Mutex::new(matching);

        (0..self.size).into_par_iter().for_each(
            |first_piece_index| {
                for second_piece_index in first_piece_index + 1..self.size {
                    for first_piece_orientation in 0..4 {
                        for second_piece_orientation in 0..4 {

                            let first_piece = &self.pieces[first_piece_index][first_piece_orientation];
                            let second_piece = &self.pieces[second_piece_index][second_piece_orientation];

                            if first_piece.compare_to(Direction::UP, second_piece, 0, 0).get_shore() != 0 {
                                matching_mutex.lock().unwrap()[first_piece_index].insert_piece_that_match(first_piece_orientation, second_piece);
                            }
                        }
                    }
                }
            }
        );

        let matching = matching_mutex.into_inner().unwrap();
        return MatchForGroupPieceHolder{
            matching
        }
    }*/
}

/// reference one piece inside `PieceGroupHolder` keeping his index, orientation and reference
pub struct PieceRef<'a,T>{
    pub index: usize,
    pub orientation: usize,
    pub reference: &'a T,
    pub shore: Shore
}

impl<'a,T> Debug for PieceRef<'a,T>{
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "[index: {} orientation: {}]", self.index,self.orientation)
    }
}

impl<'a,T> PieceRef<'a,T> {
    pub fn new(index: usize, orientation: usize, reference: &'a T,shore: Shore) -> Self{
        return Self{
            index,
            orientation,
            reference,
            shore
        }
    }
}
#[derive(Debug)]
pub struct MatchesForOnePiece<'a,T>{
    /// for each of the 4 directions, this vector contains the pieces that can match
    matches: [Vec<PieceRef<'a,T>>;4],
    /// for each of the 4 directions, this vector mapp one of the indexes of the piece
    /// to the index of the matches array that contains that contains a match with an index
    /// higher that the original index
    /// this is used because in the final version all pieces insert must have an index that is
    /// higher that the index of the first piece. to avoid duplications
    map_piece_index_to_aray_begin: [Vec<usize>;4]
}

impl<'a,T> MatchesForOnePiece<'a,T> {

    pub fn get_matches(&self, direction: Direction) -> &[PieceRef<T>]{
        match direction {
            Direction::UP => self.matches[0].as_slice(),
            Direction::RIGHT => self.matches[1].as_slice(),
            Direction::DOWN => self.matches[2].as_slice(),
            Direction::LEFT => self.matches[3].as_slice(),
        }
    }

    /// return slice of matches
    pub fn get_matches_with_higher_index(&self, direction: Direction, base_index: usize) -> &[PieceRef<T>]{
        let (slize, map) = match direction {
            Direction::UP    => (self.matches[0].as_slice(), self.map_piece_index_to_aray_begin[0].as_slice()),
            Direction::RIGHT => (self.matches[1].as_slice(), self.map_piece_index_to_aray_begin[1].as_slice()),
            Direction::DOWN  => (self.matches[2].as_slice(), self.map_piece_index_to_aray_begin[2].as_slice()),
            Direction::LEFT  => (self.matches[3].as_slice(), self.map_piece_index_to_aray_begin[3].as_slice()),
        };
        return &slize[map[base_index]..]
    }

    pub fn insert_match(&mut self, to_insert: PieceRef<'a,T>, direction: Direction){
        match direction {
            Direction::UP => self.matches[0].push(to_insert),
            Direction::RIGHT => self.matches[1].push(to_insert),
            Direction::DOWN => self.matches[2].push(to_insert),
            Direction::LEFT => self.matches[3].push(to_insert),
        };
    }

    pub fn new() -> Self{
        Self{
            matches: [vec![],vec![],vec![],vec![]],
            map_piece_index_to_aray_begin: [vec![],vec![],vec![],vec![]]
        }
    }

    /// finalize the object by building `piece_index_to_aray_begin`
    pub fn finalize(&mut self, number_of_pieces: usize){

        let v = Vec::with_capacity(number_of_pieces);
        self.map_piece_index_to_aray_begin = [v.clone(),v.clone(),v.clone(),v];

        for direction in 0..4{

            let slice_matches = &self.matches[direction];
            let slice_map = &mut self.map_piece_index_to_aray_begin[direction];

            let size_matches = slice_matches.len();

            let mut index_matches = 0;

            for index_map in 0..number_of_pieces{
                loop {
                    if index_matches == size_matches{
                        break;
                    }

                    if slice_matches[index_matches].index > index_map{
                        break;
                    }

                    index_matches+=1;
                }

                slice_map.push(index_matches)
            }
        }

    }
}


#[derive(Debug)]
pub struct MatchForOnePieceAllOrientation<'a,T>{
    /// keep the possible match depending on all the possible orientations of the current piece
    matches: [MatchesForOnePiece<'a,T>;4]
}

impl<'a,T: Clone + HasOrientation + Send + Sync + Comparable> MatchForOnePieceAllOrientation<'a,T> {

    /// the piece that will be insert must match with the piece this struct represent, on the UP direction
    pub fn insert_match(&mut self, this_piece_orientation: usize, to_insert_index: usize, to_insert_orientation: usize, pgh: &'a PieceGroupHolder<T>, shore: Shore){

        for (i,direction) in zip(0..4, [Direction::UP,Direction::RIGHT,Direction::DOWN, Direction::LEFT]){

            let orientation_this = (this_piece_orientation+4-i)%4;
            let orientation_other = (to_insert_orientation+4-i)%4;

            let p = PieceRef::new(to_insert_index, orientation_other, pgh.get(to_insert_index, orientation_other),shore);
            self.matches[orientation_this].insert_match(p,direction);
        }
    }

    pub fn get_matches(&self, orientation: usize, direction: Direction) -> &[PieceRef<T>]{
        self.matches[orientation].get_matches(direction)
    }
    pub fn get_matches_with_higher_index(&self, orientation: usize, direction: Direction,base_index: usize) -> &[PieceRef<T>]{
        self.matches[orientation].get_matches_with_higher_index(direction,base_index)
    }
    /// finalize the object by building `piece_index_to_aray_begin`
    pub fn finalize(&mut self, number_of_pieces: usize){
        for x in 0..4{
            self.matches[x].finalize(number_of_pieces);
        }
    }

    pub fn new() -> Self{
        Self{
            matches: [MatchesForOnePiece::new(),MatchesForOnePiece::new(),MatchesForOnePiece::new(),MatchesForOnePiece::new()]
        }
    }
}

#[derive(Debug)]
pub struct MatchForAllPieces<'a,T>{
    matches: Vec<MatchForOnePieceAllOrientation<'a,T>>
}

impl<'a,T: Clone + HasOrientation + Send + Sync + Comparable> MatchForAllPieces<'a,T> {

    /// return slice pointing to all pieces that can match the requested piece, along the requred direction
    pub fn get_matches(&self, piece_index: usize, piece_orientation: usize, direction: Direction) ->  &[PieceRef<T>]{
        self.matches[piece_index].get_matches(piece_orientation, direction)
    }
    /// same as `get_matches` but filter the slice so that only the item with index than the asked one get returnd
    pub fn get_matches_with_higher_index(&self, piece_index: usize, piece_orientation: usize, direction: Direction, base_index: usize) ->  &[PieceRef<T>]{
        self.matches[piece_index].get_matches_with_higher_index(piece_orientation, direction,base_index)
    }

    /// first piece must match on second piece on the direction UP
    pub fn insert_match(&mut self, first_index: usize, first_orientation: usize, second_index: usize, second_orientation: usize, pgh: &'a PieceGroupHolder<T>, shore: Shore){
        self.matches[first_index].insert_match(first_orientation, second_index, second_orientation, pgh, shore);
    }

    /// finalize the object by building `piece_index_to_aray_begin`
    pub fn finalize(&mut self){

        let number_of_pieces = self.matches.len();

        self.matches.par_iter_mut().for_each(
            |x|{
                x.finalize(number_of_pieces);
            }
        )
    }

    pub fn new(pgh: &'a PieceGroupHolder<T>) -> Self{
        let size = pgh.size;

        let mut matches = Vec::with_capacity(size);

        for i in 0..size{
            matches.push(MatchForOnePieceAllOrientation::<'a,T>::new());
        }

        let mut to_return = Self{ matches };

        for first_piece_index in 0..size{
            // todo: this could became first_piece_index+1..size, by reversing the orientations could make 1/2 of the operations in here
            for second_piece_index in 0..size{
                for first_piece_orientation in 0..4{
                    for second_piece_orientation in 0..4{

                        let first_piece = pgh.get(first_piece_index,first_piece_orientation);
                        let second_piece = pgh.get(second_piece_index,second_piece_orientation);

                        let shore =  first_piece.compare_to(Direction::UP,second_piece,0,0);

                        if shore.get_shore() != 0{
                            to_return.insert_match(first_piece_index,first_piece_orientation,second_piece_index,second_piece_orientation,pgh,shore);
                        }
                    }
                }
            }
        }
        to_return.finalize();
        return to_return;
    }
}

#[test]
fn test_get_match(){

    println!("Start main test!");

    use crate::{*};
    // load data for the comparator
    let init_cmp = Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_4x4\connections").unwrap();

    unsafe {
        PieceArrayWrapper::load_images_to_piece_array_wrapper(r"..\..\dataset\test_4x4\divided");
    }

    // create the vector with the basic pieces inside
    let number_of_pieces = init_cmp.get_number_of_pieces();
    let mut v = Vec::<SingePiece>::with_capacity(number_of_pieces);
    for n in 0..init_cmp.get_number_of_pieces(){
        v.push(
            SingePiece::new(n as u64, 0)
        );
    }

    // convert the vector in piece group holder
    let pgh = PieceGroupHolder::new(v);

    let mfap = MatchForAllPieces::new(&pgh);


    let p0 = pgh.get(0,0);

    for index0 in 0..pgh.size{
        for orientation0 in 0..4{

            let p0 = pgh.get(index0,orientation0);

            for direction in [Direction::UP,Direction::RIGHT,Direction::LEFT,Direction::UP]{

                let matches = mfap.get_matches(index0,orientation0,direction);

                let mut last_index = 0;

                for single_match in matches{

                    if single_match.index < last_index{
                        panic!("not sorted");
                    }
                    last_index = single_match.index;

                    print!("{} ", last_index);

                    let number_of_mathc = matches.len();

                    let index1 = single_match.index;
                    let orientation1 = single_match.orientation;

                    let p1 = pgh.get(single_match.index,single_match.orientation);

                    let shore = p0.compare_to(direction,p1,0,0).get_shore();
                    if shore == 0{
                        panic!("infos not correct!");
                    }

                }
                println!();

            }
        }
    }



}