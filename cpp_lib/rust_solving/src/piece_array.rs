use std::ptr::null;
use std::ffi::CString;

struct PieceArrayWrapper;

/// IMPORTANT: this part of the code cannot be compiled with the minWG toolchain,
/// the msvc tool chain is commended
/// you can see the tool chain you have install with the command: `rustup show`
/// the tool chain `stable-x86_64-pc-windows-msvc` is the only one that has been tested, and is
/// guaranteed to work

#[link(name="../cmake-build-debug/PieceArrayLink", kind = "static")]
extern "C"{
    /// create a new piece array
    fn create_piece_array_wrapper() -> *mut PieceArrayWrapper;

    /// generate an image
    fn generate_test_image(piece_array_wrapper: *mut PieceArrayWrapper);

    /// deallocate the memory taken by the array
    fn destroy_piece_array_wrapper(to_destroy: *mut PieceArrayWrapper);

    // load the images of the piece from the specified path
    fn load_images_to_piece_array_wrapper(path: *const libc::c_char);

    // delete the images the piece had accumulated
    fn free_images_of_piece_array_wrapper();
}

#[test]
fn test_link(){
    unsafe {

        // the path where the program can find the images
        let path = CString::new(r"..\..\dataset\test_2x3\divided").expect("CString::new failed");
        let path_ptr = path.as_ptr();

        // load the images
        load_images_to_piece_array_wrapper(path_ptr);

        // create a piece array
        let pa = create_piece_array_wrapper();

        // generate an image
        generate_test_image(pa);

        // free the image in memory
        destroy_piece_array_wrapper(pa)
    }
}