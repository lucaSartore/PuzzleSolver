use std::collections::HashSet;
use crate::piece_group::{GroupCreationResult, IsSubComponent, PieceGroup};
use crate::single_piece::SingePiece;

/// a trait for all single pieces, or group of pieces that can create a set of ids
pub trait CanCreateSet<T: IsSubComponent + CanCreateSet<T>>{
    fn get_set<'a>(top_left: &'a T, top_right: &'a T, bottom_right: &'a T, bottom_left: &'a T) -> Result<HashSet<u64>,GroupCreationResult<'a, T>>;
}

impl<'b,T: IsSubComponent + CanCreateSet<T>> CanCreateSet<PieceGroup<'b,T>> for PieceGroup<'b,T>{
    fn get_set<'a>(top_left: &'a PieceGroup<'b,T>, top_right: &'a PieceGroup<'b,T>, bottom_right: &'a PieceGroup<'b,T>, bottom_left: &'a PieceGroup<'b,T>) -> Result<HashSet<u64>, GroupCreationResult<'a, PieceGroup<'b,T>>> {

        let top_left_ids = top_left.get_ids();
        let top_right_ids = top_right.get_ids();

        //if the two pieces has an element in common the group is invalid
        if !top_left_ids.is_disjoint(top_right_ids) {
            return Result::Err(GroupCreationResult::TopRightImpossibleCombination);
        }

        // create the set of the group we are creating
        let mut ids = top_left_ids.clone();
        ids.extend(top_right_ids);

        let bottom_right_ids = bottom_right.get_ids();

        //if the two pieces has an element in common the group is invalid
        if !ids.is_disjoint(bottom_right_ids) {
            return Result::Err(GroupCreationResult::BottomRightImpossibleCombination);
        }

        //create the set of the group we are creating
        ids.extend(top_right_ids);

        let bottom_left_ids = bottom_left.get_ids();

        //if the two pieces has an element in common the group is invalid
        if !ids.is_disjoint(bottom_left_ids) {
            return Result::Err(GroupCreationResult::BottomLeftImpossibleCombination);
        }

        //create the set of the group we are creating
        ids.extend(bottom_left_ids);

        return Ok(ids);
    }
}




impl CanCreateSet<SingePiece> for SingePiece {
    fn get_set<'a>(top_left: &'a SingePiece, top_right: &'a SingePiece, bottom_right: &'a SingePiece, bottom_left: &'a SingePiece) -> Result<HashSet<u64>, GroupCreationResult<'a, SingePiece>> {

        if top_left.get_id() == top_right.get_id(){
            return Result::Err(GroupCreationResult::TopRightImpossibleCombination);
        }

        //if they have the same id, return an error
        if top_left.get_id() == bottom_right.get_id() ||  top_right.get_id() == bottom_right.get_id(){
            return Result::Err(GroupCreationResult::BottomRightImpossibleCombination);
        }

        //if they have the same id, return an error
        if top_left.get_id() == bottom_left.get_id() ||  top_right.get_id() == bottom_left.get_id() || bottom_right.get_id() == bottom_left.get_id(){
            return Result::Err(GroupCreationResult::BottomLeftImpossibleCombination);
        }

        let ids = HashSet::from(
            [
                top_left.get_id(),
                top_right.get_id(),
                bottom_right.get_id(),
                bottom_left.get_id()
            ]
        );

        return Ok(ids);
    }
}