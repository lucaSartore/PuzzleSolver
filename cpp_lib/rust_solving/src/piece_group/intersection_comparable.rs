use crate::piece_array::PieceArray;
use crate::piece_group::{Direction, HasKnownLevel, IsSubComponent, PieceArrayFiller, PieceGroup};
use crate::shore::Shore;
use crate::single_piece::SingePiece;
use super::Comparable;

pub trait IntersectionComparable: Comparable{
    fn compare_to_with_intersection(&self, direction: Direction,other: &Self,intersection_width: usize) -> Shore;
}

impl IntersectionComparable for SingePiece {
    fn compare_to_with_intersection(&self, direction: Direction, other: &Self, intersection_width: usize) -> Shore {
        if intersection_width == 0{
            self.compare_to(direction,other,0,0)
        }else if intersection_width == 1{
            if self.get_id() == other.get_id() && self.get_orientation() == other.get_orientation(){
                Shore::with_shore(255)
            }else{
                Shore::with_shore(0)
            }
        }else{
            panic!("Comparing with an intersection that is longer that the piece itself is impossible")
        }
    }
}

impl<T:Comparable + Clone + IsSubComponent + PieceArrayFiller + HasKnownLevel> IntersectionComparable for PieceGroup<'_,T> {
    fn compare_to_with_intersection(&self, direction: Direction, other: &Self, intersection_width: usize) -> Shore {
        if intersection_width == 0{
            return self.compare_to(direction,other,0,0)
        }
        if intersection_width > PieceGroup::<T>::SIDE_LEN as usize{
            panic!("Comparing with an intersection that is longer that the piece itself is impossible")
        }

        //let mut pa = PieceArray::new(size_x,size_y);
        //piece_group.fill_piece_array(&mut pa,0,0,0);
        todo!()
    }
}