cmake_minimum_required(VERSION 3.24)


project( PieceClass )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(
        PieceClass

        calculate_connections.cpp

        graphic_piece/Side.cpp
        graphic_piece/Side.h
        graphic_piece/PieceShape.cpp
        graphic_piece/PieceShape.h
        graphic_piece/util_piece.cpp
        graphic_piece/util_piece.h

        logic_piece/PieceConnections.cpp
        logic_piece/PieceConnection.h

        groped_pieces/GroupedPieces.tpp
        groped_pieces/GroupedPieces.h
        groped_pieces/Direction.cpp
        groped_pieces/Direction.h
        groped_pieces/Shore.cpp
        groped_pieces/Shore.h

        puzzle/Holder.cpp
        puzzle/Holder.h
        puzzle/PieceArray.cpp
        puzzle/PieceArray.h


)
target_link_libraries( PieceClass ${OpenCV_LIBS} )