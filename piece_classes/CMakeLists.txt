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
        logic_piece/PieceConnections.h

        full_puzzle_graph/PieceNode.h
        full_puzzle_graph/PieceNode.cpp
        full_puzzle_graph/PuzzleGraph.h
        full_puzzle_graph/PuzzleGraph.cpp
)
target_link_libraries( PieceClass ${OpenCV_LIBS} )