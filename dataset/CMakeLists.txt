cmake_minimum_required(VERSION 3.24)


project( Processing )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(
        Processing

        dataset_processing.h
        dataset_processing.cpp

        util.h
        util.cpp

        piece_splitting.h
        piece_splitting.cpp

        corner_finding.h
        corner_finding.cpp

        #../piece_classes/logic_piece/PieceConnections.h
        #../piece_classes/logic_piece/PieceConnections.cpp
)
target_link_libraries( Processing ${OpenCV_LIBS} )