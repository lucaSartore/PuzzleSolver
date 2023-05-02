cmake_minimum_required(VERSION 3.24)


project( PieceClass )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(
        PieceClass

        calculate_connections.cpp
        util.h

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
        groped_pieces/grouped_pieces_errors.h
        groped_pieces/GroupedPiecesHolder.tpp
        groped_pieces/GroupedPiecesHolder.h

        groped_pieces/impossible_combinations_graph/ImpossibleCombinationsGraph.cpp
        groped_pieces/impossible_combinations_graph/ImpossibleCombinationsGraph.h
        groped_pieces/impossible_combinations_graph/Node.cpp
        groped_pieces/impossible_combinations_graph/GroupedPiecesNode.h

        puzzle_preview/PreviewHolder.cpp
        puzzle_preview/PreviewHolder.h
        puzzle_preview/PieceArray.tpp
        puzzle_preview/PieceArray.h
        puzzle_preview/PieceImmage.cpp
        puzzle_preview/PieceImage.h
        puzzle_preview/ShoringHolder.cpp
        puzzle_preview/ShoringHolder.h
        puzzle_preview/Holder.cpp
        puzzle_preview/Holder.h
        puzzle_preview/point_indexes.h



)
target_link_libraries( PieceClass ${OpenCV_LIBS} )