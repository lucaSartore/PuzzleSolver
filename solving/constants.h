//
// Created by luca on 5/2/23.
//

#ifndef PIECECLASS_CONSTANTS_H
#define PIECECLASS_CONSTANTS_H

/// this is the maximum level of group pieces that will be compiled.
/// this define the biggest size of puzzle that ca be solved
/// for exaple if N = 10 the side of the puzzle has to be at most
/// 2^N = 1024 pieces, so the maximum number of pieces will be about 1M
#define MAX_GroupedPieces_LEVEL 10

/// the minimum compatibility a piece must have in order to be considered a valid connection when using the
/// calculate_connection function
#define MIN_COMPAT_CC 0.45


#endif //PIECECLASS_CONSTANTS_H

