cmake_minimum_required(VERSION 3.24)


project( PieceClass )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( PieceClass calculate_connections.cpp grafic_piece/Side.cpp grafic_piece/Side.h grafic_piece/PieceShape.cpp grafic_piece/PieceShape.h grafic_piece/util_piece.cpp grafic_piece/util_piece.h logic_piece/PieceConnections.cpp full_puzzle_graph/PieceNode.h full_puzzle_graph/PieceNode.cpp)
target_link_libraries( PieceClass ${OpenCV_LIBS} )