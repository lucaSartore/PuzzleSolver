cmake_minimum_required(VERSION 3.24)


project( PieceClass )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( PieceClass testing.cpp grafic_piece/Side.cpp grafic_piece/Side.h grafic_piece/Piece.cpp grafic_piece/Piece.h grafic_piece/util_piece.cpp grafic_piece/util_piece.h)
target_link_libraries( PieceClass ${OpenCV_LIBS} )