use rayon::prelude::*;
use crate::piece_group::HasOrientation;
/// this struct is used to keep all of the possible combination of one rotation of piece array group in scope
pub struct PieceGroupHolder<T: Clone + HasOrientation + Send>{
    // the 4 vec represent the 4 orientation, each element represent a specific piece
    pieces: [Vec<T>;4],
    size: usize
}

impl<T: Clone + HasOrientation + Send> PieceGroupHolder<T>{
    /// get a vector with the pieces inside, and create all the possible orientations of ir
    pub fn new(vec: Vec<T>) -> Self{

        let size = vec.len();

        let mut pieces = [vec.clone(),vec.clone(),vec.clone(),vec];

        for i in 0..4{
            pieces[i].par_iter_mut().for_each(|x| x.set_orientation(i as u64));
        }

        return Self{
            pieces,
            size
        }
    }
    pub fn get(&self, id: usize, orientation: usize) -> &T{
        &(self.pieces[orientation][id])
    }

    pub fn get_size(&self)->usize{
        return self.size;
    }
}