use std::collections::HashSet;
use crate::piece_basics_components::PieceBasicComponents;
use crate::piece_group::{GroupCreationResult, IsSubComponent, PieceGroup};
use crate::single_piece::SingePiece;

/// a trait for all single pieces, or group of pieces that can create a set of ids
pub trait CanCreateBasicComponents<T: IsSubComponent + CanCreateBasicComponents<T>>{
    fn get_set<'a>(top_left: &'a T, top_right: &'a T, bottom_right: &'a T, bottom_left: &'a T) -> Result<PieceBasicComponents,GroupCreationResult<'a, T>>;
}

impl<'b,T: IsSubComponent + CanCreateBasicComponents<T>> CanCreateBasicComponents<PieceGroup<'b,T>> for PieceGroup<'b,T>{
    fn get_set<'a>(top_left: &'a PieceGroup<'b,T>, top_right: &'a PieceGroup<'b,T>, bottom_right: &'a PieceGroup<'b,T>, bottom_left: &'a PieceGroup<'b,T>) -> Result<PieceBasicComponents, GroupCreationResult<'a, PieceGroup<'b,T>>> {

        let top_left_components = top_left.get_basic_components();
        let top_right_components = top_right.get_basic_components();

        let mut result = match PieceBasicComponents::merge(top_left_components,top_right_components){
            Ok(e) => e,
            Err(_) => return Result::Err(GroupCreationResult::TopRightImpossibleCombination)
        };

        let bottom_right_components = bottom_right.get_basic_components();

        result = match PieceBasicComponents::merge(&result,bottom_right_components){
            Ok(e) => e,
            Err(_) => return Result::Err(GroupCreationResult::BottomRightImpossibleCombination)
        };


        let bottom_left_components = bottom_left.get_basic_components();

        result = match PieceBasicComponents::merge(&result,bottom_left_components){
            Ok(e) => e,
            Err(_) => return Result::Err(GroupCreationResult::BottomLeftImpossibleCombination)
        };

        return Ok(result);
    }
}




impl CanCreateBasicComponents<SingePiece> for SingePiece {
    fn get_set<'a>(top_left: &'a SingePiece, top_right: &'a SingePiece, bottom_right: &'a SingePiece, bottom_left: &'a SingePiece) -> Result<PieceBasicComponents, GroupCreationResult<'a, SingePiece>> {

        if top_left.get_id() == top_right.get_id(){
            return Result::Err(GroupCreationResult::TopRightImpossibleCombination);
        }

        //if they have the same id, return an error
        if top_left.get_id() == bottom_right.get_id() ||  top_right.get_id() == bottom_right.get_id(){
            return Result::Err(GroupCreationResult::BottomRightImpossibleCombination);
        }

        //if they have the same id, return an error
        if top_left.get_id() == bottom_left.get_id() || top_right.get_id() == bottom_left.get_id() || bottom_right.get_id() == bottom_left.get_id(){
            return Result::Err(GroupCreationResult::BottomLeftImpossibleCombination);
        }

        // build the component with the elements to return
        let mut to_return = PieceBasicComponents::merge(&top_left.basic_components,&top_right.basic_components).unwrap();
        to_return = PieceBasicComponents::merge(&to_return,&bottom_right.basic_components).unwrap();
        to_return = PieceBasicComponents::merge(&to_return,&bottom_left.basic_components).unwrap();


        return Ok(to_return);
    }
}