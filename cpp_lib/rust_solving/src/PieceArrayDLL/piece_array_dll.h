#ifndef PUZZLE_SOLVER_PIECE_ARRAY_DLL_H
#define PUZZLE_SOLVER_PIECE_ARRAY_DLL_H

#include "iostream"

// this file allows the rust puzzle solver to access the functionalities of piece array

/// a wrapper of piece array that can be used, combined with the functions bellow to
/// access the piece array functionality from Rust
typedef struct PieceArrayWrapper PieceArrayWrapper;

/// create a new piece array
PieceArrayWrapper* create_piece_array();

/// generate an image
void generate_test_image(PieceArrayWrapper* piece_array_wrapper);

/// deallocate the memory taken by the array
void destroy_piece_array_wrapper(PieceArrayWrapper* to_destroy);

/// load the images of the piece from the specified path
void load_images_to_piece_array_wrapper(const char* path);

/// delete the images the piece had accumulated
void free_images_of_piece_array_wrapper();
#endif //PUZZLE_SOLVER_PIECE_ARRAY_DLL_H
