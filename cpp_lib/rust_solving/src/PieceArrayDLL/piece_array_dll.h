#ifndef PUZZLE_SOLVER_PIECE_ARRAY_DLL_H
#define PUZZLE_SOLVER_PIECE_ARRAY_DLL_H

// this file allows the rust puzzle solver to access the functionalities of piece array

typedef struct PieceArrayWrapper PieceArrayWrapper;

PieceArrayWrapper* create_piece_array();

void generate_test_image(PieceArrayWrapper* piece_array_wrapper);

void destroy_piece_array_wrapper(PieceArrayWrapper* to_destroy);

#endif //PUZZLE_SOLVER_PIECE_ARRAY_DLL_H
