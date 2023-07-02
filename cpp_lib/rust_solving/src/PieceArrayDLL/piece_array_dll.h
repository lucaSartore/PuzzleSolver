#ifndef PUZZLE_SOLVER_PIECE_ARRAY_DLL_H
#define PUZZLE_SOLVER_PIECE_ARRAY_DLL_H

#include "iostream"

// this file allows the rust puzzle solver to access the functionalities of piece array


// a piece and his orientation
struct SingePiece{
    uint64_t id;
    uint64_t orientation;
};

extern "C"{
    /// a wrapper of piece array that can be used, combined with the functions bellow to
    /// access the piece array functionality from Rust
    typedef struct PieceArrayWrapper PieceArrayWrapper;

    /// create a new piece array with dimensions size_x x size_y and it will be filled up with the pieces contained in the pieces array,
    /// following "reading order" (left to right, and then top to bottom)
    __declspec(dllexport) PieceArrayWrapper* create_piece_array_wrapper(uint64_t size_x, uint64_t size_y, SingePiece* pieces);

    /// generate an image
    __declspec(dllexport) void generate_test_image(PieceArrayWrapper* piece_array_wrapper);

    /// return the shore (aka how well sed pieces fit together) of an image
    __declspec(dllexport) float get_shore(PieceArrayWrapper* piece_array_wrapper);

    /// send a preview image to the c# app for the user to see
    __declspec(dllexport) void generate_preview_image(PieceArrayWrapper* piece_array_wrapper);

    /// deallocate the memory taken by the array
    __declspec(dllexport) void destroy_piece_array_wrapper(PieceArrayWrapper* to_destroy);

    /// load the images of the piece from the specified path
    __declspec(dllexport) void load_images_to_piece_array_wrapper(const char* path);

    /// delete the images the piece had accumulated
    __declspec(dllexport) void free_images_of_piece_array_wrapper();
};

#endif //PUZZLE_SOLVER_PIECE_ARRAY_DLL_H

