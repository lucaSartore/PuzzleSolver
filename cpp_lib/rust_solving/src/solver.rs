use crate::piece_group::IsSubComponent;
use crate::piece_group::HasSetInIt;
use crate::piece_group::PieceGroup;
use crate::piece_group::Comparable;
use crate::piece_group::HasOrientation;
use crate::piece_group_holder::PieceGroupHolder;

pub fn solve<T: Clone + HasOrientation + Send + Comparable + IsSubComponent>(pgh: &PieceGroupHolder<T>){
    let size = pgh.get_size();

    // the function to apply for every iteration of the loop
    let x = |top_left_index: usize| {

        // select all 3 other components one by one. start from top_left_index +1 to avoid duplicates
        'top_right_index_loop:
        for top_right_index in top_left_index..size{
            'bottom_right_index_loop:
            for bottom_right_index in top_left_index..size{
                'bottom_left_index_loop:
                for bottom_left_index in top_left_index..size{

                    // keep track of how many times i fail the placement of the piece,
                    // if for examole piece 1 and piece 2 are incompatible as top right and top left, in any possible combination
                    // then there is no need to test all combinations of bottom right and bottom left
                    let failed_top_right_placement = 0;
                    let failed_bottom_right_placement = 0;

                    'top_left_orientation_loop:
                    for top_left_orientation in 0..4{
                        'top_right_orientation_loop:
                        for top_right_orientation in 0..4{
                            'bottom_right_orientation_loop:
                            for bottom_right_orientation in 0..4{
                                'bottom_left_orientation_loop:
                                for bottom_left_orientation in 0..4{

                                    // get the 4 element our sub piece will be made of
                                    let top_left= pgh.get(top_left_index,top_left_orientation);
                                    let top_right= pgh.get(top_right_index,top_right_orientation);
                                    let bottom_right= pgh.get(bottom_right_index,bottom_right_orientation);
                                    let bottom_left= pgh.get(bottom_left_index,bottom_left_orientation);

                                    let pg = T::merge_together(top_left,top_right,bottom_right,bottom_left);

                                }
                            }
                        }
                    }
                }
            }
        }

    };

}