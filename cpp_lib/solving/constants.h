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
#define MIN_COMPAT_CC 0.3

/// the minimum shore a PieceArray<ShoringHolder> needs to have, in order to be consider valid
#define MIN_SHORE_PIECE_ARRAY 0.975

/// once a new group is created, the avrege shore of his connections is calculated, if the avrege
/// is hirer than this constant, the group gets considered valid
#define MIN_AVG_SHORE_FOR_GROUP 0.5

/// how much the original image will be resized for the preview
#define PREVIEW_RESIZE_FACTOR 4
/// the resolution of the image generated when a piece is rotated ( need to be big enough to fit a puzzle piece inside)
#define ROTATION_RESOLUTION 2000

#endif //PIECECLASS_CONSTANTS_H

