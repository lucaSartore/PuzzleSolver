cmake_minimum_required(VERSION 3.24)


project( PieceClass )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( PieceClass testing.cpp Side.cpp Side.h Piece.cpp Piece.h)
target_link_libraries( PieceClass ${OpenCV_LIBS} )