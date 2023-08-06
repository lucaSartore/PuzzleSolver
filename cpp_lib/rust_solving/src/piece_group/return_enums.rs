use std::fmt::Debug;
use crate::piece_group::{Comparable, IsSubComponent, PieceGroup};
use crate::shore::Shore;

/// when trying to create a new group of piece many things can go wrong, this enum is the result
/// that represent all the errors, as well as the solution
#[derive(Debug)]
pub enum GroupCreationResult<'a, T: Comparable + Clone + IsSubComponent>{
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

impl<'a, T: Comparable + Debug + Clone + IsSubComponent> GroupCreationResult<'a, T>{
    pub fn unwrap(self) -> PieceGroup<'a, T>{
        if let Self::Ok(e) = self{
            return e
        }
        panic!("call unwrap, bur value was: {:?}", self)
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
