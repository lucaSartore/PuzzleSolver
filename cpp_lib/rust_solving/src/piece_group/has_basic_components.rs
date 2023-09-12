use std::collections::HashSet;
use crate::piece_basics_components::PieceBasicComponents;
use crate::piece_group::{GroupCreationResult, IsSubComponent, PieceGroup};
use crate::piece_group_holder::PieceRef;
use crate::single_piece::SingePiece;

/// a trait for all single pieces, or group of pieces that can create a set of ids
pub trait HasBasicComponents<T: IsSubComponent + HasBasicComponents<T>>{
    fn get_basic_components(&self) -> &PieceBasicComponents;
    fn merge_basic_components<'a>(top_left: &'a T, top_right: &PieceRef<'a,T>, bottom_right: &PieceRef<'a,T>, bottom_left: &PieceRef<'a,T>) -> Result<PieceBasicComponents,GroupCreationResult<'a, T>>;
}

impl<'b,T: IsSubComponent + HasBasicComponents<T>> HasBasicComponents<PieceGroup<'b,T>> for PieceGroup<'b,T>{
    fn get_basic_components(&self) -> &PieceBasicComponents {
        &self.basic_components
    }

    fn merge_basic_components<'a>(top_left: &'a PieceGroup<'b,T>, top_right: &PieceRef<'a,PieceGroup<'b,T>>, bottom_right: &PieceRef<'a,PieceGroup<'b,T>>, bottom_left: &PieceRef<'a,PieceGroup<'b,T>>) -> Result<PieceBasicComponents, GroupCreationResult<'a, PieceGroup<'b,T>>> {

        let top_left_components = top_left.get_basic_components();
        let top_right_components = top_right.reference.get_basic_components();

        let mut result = match PieceBasicComponents::merge(top_left_components,top_right_components){
            Ok(e) => e,
            Err(_) => return Result::Err(GroupCreationResult::TopRightImpossibleCombination)
        };

        let bottom_right_components = bottom_right.reference.get_basic_components();

        result = match PieceBasicComponents::merge(&result,bottom_right_components){
            Ok(e) => e,
            Err(_) => return Result::Err(GroupCreationResult::BottomRightImpossibleCombination)
        };


        let bottom_left_components = bottom_left.reference.get_basic_components();

        result = match PieceBasicComponents::merge(&result,bottom_left_components){
            Ok(e) => e,
            Err(_) => return Result::Err(GroupCreationResult::BottomLeftImpossibleCombination)
        };

        return Ok(result);
    }
}




impl HasBasicComponents<SingePiece> for SingePiece {
    fn get_basic_components(&self) -> &PieceBasicComponents {
        &self.basic_components
    }
    fn merge_basic_components<'a>(top_left: &'a SingePiece, top_right: &PieceRef<'a,Self>, bottom_right: &PieceRef<'a,Self>, bottom_left: &PieceRef<'a,Self>) -> Result<PieceBasicComponents, GroupCreationResult<'a, SingePiece>> {


        // the compatibility between top_left and the other is ensured by de fact that the top_left always has the higher index in the group
        // the compatibility between top_right and bottom_right is checked when building MatchForAllPieces
        // the compatibility between bottom_right and bottom_left is checked when building MatchForAllPieces

        // the only thing that remains to check is te compatibility between bottom_left and top_right

        if bottom_left.reference.get_id() == top_right.reference.get_id(){
            return Result::Err(GroupCreationResult::BottomRightImpossibleCombination);
        }

        /*
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
        }*/

        // build the component with the elements to return
        // top_right's basic components are merged with top left, and bottom left's basic components are merged with bottom right, so in this way you get
        // the entire thing
        let mut to_return = PieceBasicComponents::merge(&top_right.basic_components,&bottom_left.basic_components).unwrap();

        return Ok(to_return);
    }
}