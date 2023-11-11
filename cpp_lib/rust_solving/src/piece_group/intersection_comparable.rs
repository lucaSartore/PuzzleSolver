use crate::piece_array::piece_array_slice::PieceArraySlice;
use crate::piece_array::PieceArray;
use crate::piece_group::{Direction, HasKnownLevel, IsSubComponent, PieceArrayFiller, PieceGroup};
use crate::shore::Shore;
use crate::single_piece::SingePiece;
use super::Comparable;

/// at the last step of the puzzle it is likely that a normal comparison won't work
/// (given that the puzzle is not always a prefect square with a side that is a power of two)
/// this trait is used to compare two pieces assuming that tey have an intersection with each other.
pub trait IntersectionComparable: Comparable{
    fn compare_to_with_intersection(&self, direction: Direction,other: &Self,intersection_width: u64) -> Shore;
}

impl IntersectionComparable for SingePiece {
    fn compare_to_with_intersection(&self, direction: Direction, other: &Self, intersection_width: u64) -> Shore {
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
    fn compare_to_with_intersection(&self, direction: Direction, other: &Self, intersection_width: u64) -> Shore {

        let side_len = PieceGroup::<T>::SIDE_LEN;

        if intersection_width == 0{
            return self.compare_to(direction,other,0,0)
        }
        if intersection_width > PieceGroup::<T>::SIDE_LEN{
            panic!("Comparing with an intersection that is longer that the piece itself is impossible")
        }

        // create the piece array slice
        let mut pa_self = PieceArray::new(side_len, side_len);
        self.fill_piece_array(&mut pa_self, 0, 0, 0);
        let mut pa_other = PieceArray::new( side_len, side_len);
        other.fill_piece_array(&mut pa_other,0,0,0);

        fn get_slice(pa: &PieceArray, direction: Direction, width: u64) -> PieceArraySlice{
            let x_min;
            let x_max;
            let y_min;
            let y_max;

            match direction {
                Direction::UP =>{
                    x_min = 0;
                    x_max = pa.dim_x-1;
                    y_min = 0;
                    y_max = width;
                }
                Direction::RIGHT =>{
                    x_min = pa.dim_x-1-width;
                    x_max = pa.dim_x-1;
                    y_min = 0;
                    y_max = pa.dim_y-1;
                }
                Direction::DOWN =>{
                    x_min = 0;
                    x_max = pa.dim_x-1;
                    y_min = pa.dim_y-1-width;
                    y_max = pa.dim_y-1;
                }
                Direction::LEFT =>{
                    x_min = 0;
                    x_max = width;
                    y_min = 0;
                    y_max = pa.dim_y-1;
                }
            }
            return pa.get_slice(x_min,x_max,y_min,y_max);
        }



        let pa_slice_self = get_slice(&pa_self,direction,intersection_width);
        let pa_slice_other = get_slice(&pa_other,-direction,intersection_width);

        if pa_slice_self == pa_slice_other {
            Shore::with_shore(255)
        } else {
            Shore::with_shore(0)
        }
    }
}