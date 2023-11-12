

#[allow(dead_code)]
mod shore;
#[allow(dead_code)]
mod piece_array;
#[allow(dead_code)]
mod single_piece;
#[allow(dead_code)]
mod piece_group;
#[allow(dead_code)]
mod piece_comparing;
#[allow(dead_code)]
mod piece_group_holder;
#[allow(dead_code)]
mod solver;
#[allow(dead_code)]
mod piece_basics_components;

mod constants;

mod finalize_piece_array;

use std::collections::HashSet;
use libc::size_t;
use piece_array::PieceArrayWrapper;
use piece_comparing::{Comparator, InitializationResults, Initialized};
use piece_group::PieceGroup;
use piece_group::Comparable;
use piece_group::Direction;
use piece_group_holder::PieceGroupHolder;
use shore::Shore;
use single_piece::SingePiece;
use crate::constants::MIN_SHORE_PIECE_ARRAY;
use crate::piece_array::PieceArray;
use crate::piece_group::PieceArrayFiller;
use crate::solver::TEST;
use std::ffi::{c_void, CStr};
use std::ptr::null;
use crate::piece_basics_components::PieceBasicComponents;

// type of a function pointer in c that is used for the preview callback
type CallbackFunc = unsafe extern "C" fn(*mut PieceArrayWrapper);
static mut CALL_BACK_FUNC: CallbackFunc = null_func;
pub unsafe extern "C" fn null_func(paw: *mut PieceArrayWrapper){}

#[no_mangle]
pub extern "C" fn solve_puzzle_rust(path_images: *const libc::c_char,path_connections: *const libc::c_char,path_output: *const libc::c_char, size_x: u32, size_y: u32, callback_func: CallbackFunc) -> bool{

    // initialize the call back function
    unsafe {
        CALL_BACK_FUNC = callback_func;
    }

    // convert all the strings in c string
    let path_images = unsafe { CStr::from_ptr(path_images) };
    let path_images = path_images.to_str().unwrap();

    let path_connections = unsafe { CStr::from_ptr(path_connections) };
    let path_connections = path_connections.to_str().unwrap();

    let path_output = unsafe { CStr::from_ptr(path_output) };
    let path_output = path_output.to_str().unwrap();

    // load data for the comparator
    let init_cmp = Comparator::<Initialized>::initialize_comparator(path_connections).unwrap();

    // init the images
    unsafe {
        PieceArrayWrapper::load_images_to_piece_array_wrapper(path_images);
    }

    // create the vector with the basic pieces inside
    let number_of_pieces = init_cmp.get_number_of_pieces();
    PieceBasicComponents::initialize(number_of_pieces);
    let mut v = Vec::<SingePiece>::with_capacity(number_of_pieces);
    for n in 0..init_cmp.get_number_of_pieces(){
        v.push(
            SingePiece::new(n as u64, 0)
        );
    }

    // convert the vector in piece group holder
    let pgh = PieceGroupHolder::new(v);

    // call the solving function
    return solver::solve(&pgh,path_output,size_x as u64,size_y as u64);
}

#[test]
fn main_test(){

    println!("Start main test!");




    /*const CONNECTIONS_PATH: &str = r"..\..\dataset\test_4x4\connections";
    const DIVIDED_PATH: &str = r"..\..\dataset\test_4x4\divided";
    const SIZE_X: u64 = 4;
    const SIZE_Y: u64 = 4;*/

    /*
    const CONNECTIONS_PATH: &str = r"..\..\dataset\test_8x8\connections";
    const DIVIDED_PATH: &str = r"..\..\dataset\test_8x8\divided";
    const SIZE_X: u64 = 8;
    const SIZE_Y: u64 = 8;*/


    const CONNECTIONS_PATH: &str = r"..\..\dataset\test_4x4_digital\connections";
    const DIVIDED_PATH: &str = r"..\..\dataset\test_4x4_digital\divided";
    const SIZE_X: u64 = 4;
    const SIZE_Y: u64 = 4;


    /*
    const CONNECTIONS_PATH: &str = r"..\..\dataset\test_5x7_digital\connections";
    const DIVIDED_PATH: &str = r"..\..\dataset\test_5x7_digital\divided";
    const SIZE_X: u64 = 5;
    const SIZE_Y: u64 = 7;*/

    /*
    const CONNECTIONS_PATH: &str = r"..\..\dataset\test_16x16_digital\connections";
    const DIVIDED_PATH: &str = r"..\..\dataset\test_16x16_digital\divided";
    const SIZE_X: u64 = 16;
    const SIZE_Y: u64 = 16;*/

    use crate::{*};
    // load data for the comparator
    let init_cmp = Comparator::<Initialized>::initialize_comparator(CONNECTIONS_PATH).unwrap();

    unsafe {
        PieceArrayWrapper::load_images_to_piece_array_wrapper(DIVIDED_PATH);
    }

    // create the vector with the basic pieces inside
    let number_of_pieces = init_cmp.get_number_of_pieces();
    PieceBasicComponents::initialize(number_of_pieces);
    let mut v = Vec::<SingePiece>::with_capacity(number_of_pieces);
    for n in 0..init_cmp.get_number_of_pieces(){
        v.push(
            SingePiece::new(n as u64, 0)
        );
    }

    // convert the vector in piece group holder
    let pgh = PieceGroupHolder::new(v);

    solver::solve(&pgh,".\\",SIZE_X,SIZE_Y);

}