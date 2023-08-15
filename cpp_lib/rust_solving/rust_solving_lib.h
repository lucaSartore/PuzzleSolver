#ifndef RUST_SOLVING_PUZZLESOLVER_H
#define RUST_SOLVING_PUZZLESOLVER_H

#include <stdint.h>
#include "stdint.h"

#include "rust_solving/src/PieceArrayDLL/piece_array_dll.h"

extern "C"{
    __declspec(dllimport) bool solve_puzzle_rust(const char* path_images ,const char* path_connections,const char* path_output,uint32_t size_x, uint32_t size_y, void (*callback_func)(PieceArrayWrapper*));
}

#endif