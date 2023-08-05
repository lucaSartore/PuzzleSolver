use libc::remove;
use crate::constants::MIN_SHORE_PIECE_ARRAY;
use crate::piece_array::PieceArray;
use crate::piece_group::NextLevelOrPanic;
use crate::piece_group_holder::PieceGroupHolder;

/// this function takes as input a list of piece groups (that by nature have shape N^2 x N^2 with N positive integer)
/// and turns it into a piece array with an arbitrary dimension
/// Note: N^2 * 2 must be greater or equal of both size_x and size_y in order for this function to work
/// puzzle with a wide aspect ration (greater that 2:1) are not supported by this function)
pub fn finalize_piece_array<T: NextLevelOrPanic>(pgh: &PieceGroupHolder<T>, output_path: &str ,size_x: u64, size_y: u64) -> bool{

    let mut best: Option<PieceArray> = None;
    let best_shore = 0.0;
    // nota: for now this function only solve squared puzzles
    for piece_id in 0..pgh.get_size(){
        // calculate the shore of the current piece
        let piece_group = pgh.get(piece_id,0);
        let mut pa = PieceArray::new(size_x,size_y);
        piece_group.fill_piece_array(&mut pa,0,0,0);
        unsafe {
            // call c++ dll
            let mut paw = pa.get_piece_array_wrapper();

            // get the shore of the  current piece array
            let shore = (*paw).get_shore();

            // keep track of the best shore
            if shore > best_shore{
                best = Option::Some(pa);
            }

            // deallocate memory
            (*paw).destroy_piece_array_wrapper();
        }

    }
    // get the best result, or return false to signal that the puzzle hasn't been solved
    let mut best = match best {
        Option::Some(e) => e,
        Option::None => return false
    };
    unsafe {
        // call c++ dll
        let mut paw = best.get_piece_array_wrapper();

        // save the image
        (*paw).generate_test_image(format!("{}\\result.png",output_path).as_str());

        // save the binary
        (*paw).save_as_binary(format!("{}\\result.bin",output_path).as_str());

        // deallocate memory
        (*paw).destroy_piece_array_wrapper();
    }


    return true;
}