use std::cell::RefCell;
use std::sync::Mutex;
use std::time::Duration;
use libc::tolower;
use rayon::prelude::*;
use crate::piece_group::{Comparable, Direction, HasOrientation};


/// this struct is associated to one single pieces, and contain a list of all the pieces that can match
/// with it for each side
#[derive(Debug, Clone)]
struct PieceThatMatchForEverySide<'a, T>{
    // contain list of pointers that can mathc with the current piece for each of the 4 sides
    matching: [Vec<&'a T>;4]
}


impl<'a,T> PieceThatMatchForEverySide<'a,T> {
    pub fn get_pieces_that_match(&'a self, orientation_piece: usize) -> &'a[&'a T]{
        &self.matching[orientation_piece].as_slice()
    }

    pub fn insert_piece_that_match(&mut self, orientation_piece: usize, piece_to_insert: &'a T){
        self.matching[orientation_piece].push(piece_to_insert);
    }

    pub fn new() -> Self{
        Self{
            matching: [vec![],vec![],vec![],vec![]]
        }
    }
}


/// this struct is used to keep all of the possible combination of one rotation of piece array group in scope
pub struct PieceGroupHolder<'a,T: Clone + HasOrientation + Send + Sync + Comparable>{
    /// the 4 vec represent the 4 orientation, each element represent a specific piece
    pieces: [Vec<T>;4],
    size: usize,
    matching: Vec<PieceThatMatchForEverySide<'a,T>>
}



impl<'a,T: Clone + HasOrientation + Send + Sync + Comparable> PieceGroupHolder<'a,T>{
    /// get a vector with the pieces inside, and create all the possible orientations of ir
    pub fn new(vec: Vec<T>) -> Self{

        let size = vec.len();

        let mut pieces = [vec.clone(),vec.clone(),vec.clone(),vec];

        for i in 1..4{
            pieces[i].par_iter_mut().for_each(|x| x.set_orientation(i as u64));
        }

        // create array of pieces that can mathc to speed up testing combination process
        let mut matching = Vec::with_capacity(size);

        let mut to_return = Self{
            pieces,
            size,
            matching: matching.clone()
        };

        for _ in 0..size{
            matching.push(PieceThatMatchForEverySide::new())
        }

        let matching_mutex = Mutex::new(matching);

        (0..size).into_par_iter().for_each(
            |first_piece_index| {
                for second_piece_index in first_piece_index + 1..size {
                    for first_piece_orientation in 0..4 {
                        for second_piece_orientation in 0..4 {

                            let first_piece = &to_return.pieces[first_piece_index][first_piece_orientation];
                            let second_piece = &to_return.pieces[second_piece_index][second_piece_orientation];

                            if first_piece.compare_to(Direction::UP, second_piece, 0, 0).get_shore() > 120 {
                                matching_mutex.lock().unwrap()[first_piece_index].insert_piece_that_match(first_piece_orientation, second_piece);
                            }
                        }
                    }
                }
            }
        );

        let matching =  matching_mutex.lock().unwrap().clone();



        to_return
    }
    pub fn get(&self, id: usize, orientation: usize) -> &T{
        &(self.pieces[orientation][id])
    }

    pub fn get_size(&self)->usize{
        return self.size;
    }

    pub fn get_pieces_that_match(&'a self, index_piece: usize, orientation_piece: usize) -> &'a[&'a T]{
        self.matching[index_piece].get_pieces_that_match(orientation_piece)
    }

    // fill the matching vector
    fn calculate_pieces_that_match<'b>(size: usize, pieces: &'b [Vec<T>;4]) -> Vec<PieceThatMatchForEverySide<'b,T>>{

        // create array of pieces that can mathc to speed up testing combination process
        let mut matching = Vec::with_capacity(size);
        for i in 0..size{
            matching.push(PieceThatMatchForEverySide::<'b,T>::new());
        }

        let matching_mutex = Mutex::new(matching);

        (0..size).into_par_iter().for_each(
            |first_piece_index| {
                for second_piece_index in first_piece_index + 1..size {
                    for first_piece_orientation in 0..4 {
                        for second_piece_orientation in 0..4 {

                            let first_piece = &pieces[first_piece_index][first_piece_orientation];
                            let second_piece = &pieces[second_piece_index][second_piece_orientation];

                            if first_piece.compare_to(Direction::UP, second_piece, 0, 0).get_shore() > 120 {
                                matching_mutex.lock().unwrap()[first_piece_index].insert_piece_that_match(first_piece_orientation, second_piece);
                            }
                        }
                    }
                }
            }
        );
        return matching_mutex.lock().unwrap().clone();
    }


}