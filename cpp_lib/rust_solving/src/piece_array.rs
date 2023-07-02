struct PieceArrayWrapper;

/// IMPORTANT: this part of the code cannot be compiled with the minWG toolchain,
/// the msvc tool chain is commended
/// you can see the tool chain you have install with the command: `rustup show`
/// the tool chain `stable-x86_64-pc-windows-msvc` is the only one that has been tested, and is
/// guaranteed to work
#[link(name="PieceArrayLink", kind = "static")]
extern "C"{
    /// create a new piece array
    fn create_piece_array() -> *mut PieceArrayWrapper;

    /// generate an image
    fn generate_test_image(piece_array_wrapper: *mut PieceArrayWrapper);

    /// deallocate the memory taken by the array
    fn destroy_piece_array_wrapper(to_destroy: *mut PieceArrayWrapper);

    // load the images of the piece from the specified path
    //fn load_images_to_piece_array_wrapper(path: *const libc::c_char);

    // delete the images the piece had accumulated
    //fn free_images_of_piece_array_wrapper();
}
