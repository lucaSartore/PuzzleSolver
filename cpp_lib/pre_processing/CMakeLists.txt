cmake_minimum_required(VERSION 3.23)


set (CMAKE_CXX_STANDARD 20)
project( Processing )
find_package(OpenCV REQUIRED PATHS ../../opencv)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(
        Processing

        dataset_processing.cpp

        util.h
        util.cpp

        piece_splitting.h
        piece_splitting.cpp

        corner_finding.h
        corner_finding.cpp


        ../solving/puzzle_preview/PreviewManager.cpp
        ../solving/puzzle_preview/PreviewManager.h
        ../communication/communication_image.h
        ../communication/communication_image.cpp
        #../solving/logic_piece/PieceConnections.h
        #../solving/logic_piece/PieceConnections.cpp
)
target_link_libraries( Processing ${OpenCV_LIBS} )