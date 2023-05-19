cmake_minimum_required(VERSION 3.23)


project( Test )
find_package(OpenCV REQUIRED PATHS ../../opencv)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(
        Test
        main.cpp
        tests.h
        tests.cpp

        ../solving/util.h
        ../solving/constants.h

        ../solving/graphic_piece/Side.cpp
        ../solving/graphic_piece/Side.h
        ../solving/graphic_piece/PieceShape.cpp
        ../solving/graphic_piece/PieceShape.h
        ../solving/graphic_piece/util_piece.cpp
        ../solving/graphic_piece/util_piece.h

        ../solving/logic_piece/PieceConnections.cpp
        ../solving/logic_piece/PieceConnection.h


        ../solving/groped_pieces/GroupedPieces.tpp
        ../solving/groped_pieces/GroupedPieces.h
        ../solving/groped_pieces/Direction.cpp
        ../solving/groped_pieces/Direction.h
        ../solving/groped_pieces/Shore.cpp
        ../solving/groped_pieces/Shore.h
        ../solving/groped_pieces/grouped_pieces_errors.h
        ../solving/groped_pieces/GroupedPiecesHolder.tpp
        ../solving/groped_pieces/GroupedPiecesHolder.h

        ../solving/puzzle_preview/PieceArray.cpp
        ../solving/puzzle_preview/PieceArray.h
        ../solving/puzzle_preview/PieceImmage.cpp
        ../solving/puzzle_preview/PieceImage.h
        ../solving/puzzle_preview/Holder.cpp
        ../solving/puzzle_preview/Holder.h
        ../solving/puzzle_preview/point_indexes.h
        ../solving/puzzle_preview/PreviewManager.cpp
        ../solving/puzzle_preview/PreviewManager.h
)
target_link_libraries( Test ${OpenCV_LIBS} )