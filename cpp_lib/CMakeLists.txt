cmake_minimum_required(VERSION 3.23)

project( PuzzleSolver )

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_BUILD_TYPE Release)

# REMEMBER the bundled version of opencv is compiled with Visual Studio, if you use other compilers you need to install another version your self


# REMEMBER TO ADD: YOUR_PATH\Puzzle_Solver\opencv\x64\vc15\bin\ to your system PATH
find_package(OpenCV REQUIRED PATHS  C:/Users/lucas/CLionProjects/Puzzle_Solver/opencv/)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# the source file necessary to compile piece array
set(SOURCE_FILES_PIECE_ARRAY
        solving/puzzle_preview/PieceArray.cpp
        solving/puzzle_preview/PieceArray.h
        solving/puzzle_preview/PieceImmage.cpp
        solving/puzzle_preview/PieceImage.h
        solving/puzzle_preview/Holder.cpp
        solving/puzzle_preview/Holder.h
        solving/puzzle_preview/point_indexes.h
        solving/puzzle_preview/PreviewManager.cpp
        solving/puzzle_preview/PreviewManager.h

        solving/util.h
        solving/util.cpp

        communication/communication_image.cpp
        communication/communication_image.h
        )

#the source missing from SOURCE_FILES_PIECE_ARRAY necessary to compile everything
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

        communication/puzzle_solver_dll.cpp
        communication/puzzle_solver_dll.h

        rust_solving/src/PieceArrayDLL/piece_array_dll.h
        rust_solving/src/PieceArrayDLL/piece_array_dll.cpp

        rust_solving/src/PieceArrayDLL/piece_array_dll.h
        rust_solving/src/PieceArrayDLL/piece_array_dll.cpp

        PuzzleSolver.cpp
        PuzzleSolver.h
        )

# library for the c# app build
add_library(
        PuzzleSolverLib SHARED
        communication/puzzle_solver_dll.cpp
        communication/puzzle_solver_dll.h
        ${SOURCE_FILES}
        ${SOURCE_FILES_PIECE_ARRAY}
        rust_solving/rust_solving_lib.h
)
target_link_libraries( PuzzleSolverLib ${OpenCV_LIBS} )
target_link_libraries(PuzzleSolverLib C:/Users/lucas/CLionProjects/Puzzle_Solver/dlls_output/rust_solving_lib.dll.lib)

message(${OpenCV_LIBS})

# library for rust interop build
add_library(
        PieceArrayLink SHARED
        ${SOURCE_FILES_PIECE_ARRAY}
        rust_solving/src/PieceArrayDLL/piece_array_dll.cpp
        rust_solving/src/PieceArrayDLL/piece_array_dll.h
)
target_link_libraries(PieceArrayLink ${OpenCV_LIBS} )

# test build
add_executable(
        PuzzleSolverTest
        main_test.cpp
        ${SOURCE_FILES}
        ${SOURCE_FILES_PIECE_ARRAY}
        rust_solving/rust_solving_lib.h
)
target_link_libraries(PuzzleSolverTest ${OpenCV_LIBS})
target_link_libraries(PuzzleSolverTest C:/Users/lucas/CLionProjects/Puzzle_Solver/dlls_output/rust_solving_lib.dll.lib)

# command for access functions from terminal
add_executable(
        PuzzleSolverCommand
        puzzle_solver_command.cpp
        ${SOURCE_FILES}
        ${SOURCE_FILES_PIECE_ARRAY}
        rust_solving/rust_solving_lib.h
)
target_link_libraries(PuzzleSolverCommand ${OpenCV_LIBS})
target_link_libraries(PuzzleSolverCommand C:/Users/lucas/CLionProjects/Puzzle_Solver/dlls_output/rust_solving_lib.dll.lib)


# enable testing functionality
enable_testing()

# define tests
add_test(
        NAME PuzzleSolverTest
        COMMAND $<TARGET_FILE:PuzzleSolverTest>
)