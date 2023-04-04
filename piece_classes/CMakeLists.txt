cmake_minimum_required(VERSION 3.24)


project( PieceClass )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(
        PieceClass

        calculate_connections.cpp

        piece/Side.cpp
        piece/Side.h
        piece/Piece.cpp
        piece/Piece.h
        piece/util_piece.cpp
        piece/util_piece.h

)
target_link_libraries( PieceClass ${OpenCV_LIBS} )