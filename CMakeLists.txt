cmake_minimum_required(VERSION 3.23)

project( PuzzleSolver )

# REMEMBER TO ADD: ./MinGW/etc/include/x64/mingw/bin to your system PATH
find_package(OpenCV REQUIRED PATHS opencv)
include_directories( ${OpenCV_INCLUDE_DIRS} )

#file to include
set(SOURCE_FILES

        pre_processing/corner_finding.cpp
        pre_processing/corner_finding.h
        pre_processing/piece_splitting.h
        pre_processing/piece_splitting.cpp
        pre_processing/util.h
        pre_processing/util.cpp

        solving/calculate_connections.cpp
        solving/calculate_connections.h
        solving/solve.h
        solving/solve.cpp
        solving/util.h
        solving/constants.h

        solving/graphic_piece/Side.cpp
        solving/graphic_piece/Side.h
        solving/graphic_piece/PieceShape.cpp
        solving/graphic_piece/PieceShape.h
        solving/graphic_piece/util_piece.cpp
        solving/graphic_piece/util_piece.h

        solving/logic_piece/PieceConnections.cpp
        solving/logic_piece/PieceConnection.h


        solving/groped_pieces/GroupedPieces.tpp
        solving/groped_pieces/GroupedPieces.h
        solving/groped_pieces/Direction.cpp
        solving/groped_pieces/Direction.h
        solving/groped_pieces/Shore.cpp
        solving/groped_pieces/Shore.h
        solving/groped_pieces/grouped_pieces_errors.h
        solving/groped_pieces/GroupedPiecesHolder.tpp
        solving/groped_pieces/GroupedPiecesHolder.h

        solving/puzzle_preview/PreviewHolder.cpp
        solving/puzzle_preview/PreviewHolder.h
        solving/puzzle_preview/PieceArray.tpp
        solving/puzzle_preview/PieceArray.h
        solving/puzzle_preview/PieceImmage.cpp
        solving/puzzle_preview/PieceImage.h
        solving/puzzle_preview/ShoringHolder.cpp
        solving/puzzle_preview/ShoringHolder.h
        solving/puzzle_preview/Holder.cpp
        solving/puzzle_preview/Holder.h
        solving/puzzle_preview/point_indexes.h

        PuzzleSolver.cpp
        PuzzleSolver.h
        )


# library build
add_library(PuzzleSolverLib SHARED puzzle_solver_library.cpp puzzle_solver_library.h ${SOURCE_FILES})
target_link_libraries( PuzzleSolverLib ${OpenCV_LIBS} )

# test build
add_executable(PuzzleSolverTest main_test.cpp ${SOURCE_FILES})
target_link_libraries( PuzzleSolverTest ${OpenCV_LIBS} )

# enable testing functionality
enable_testing()

# define tests
add_test(
        NAME PuzzleSolverTest
        COMMAND $<TARGET_FILE:PuzzleSolverTest>
)