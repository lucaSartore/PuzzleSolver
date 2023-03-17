//
// Created by luca on 3/16/23.
//

#ifndef PROCESSING_PIECE_SPLITTING_H
#define PROCESSING_PIECE_SPLITTING_H

#include <opencv2/opencv.hpp>
#include <iostream>


/// this function take as input a input_path where some scansion of a puzzle is made
/// and split them into many single pieces
void split_pieces_into_single_images(const std::string& input_path,const std::string& output_path, int ppi = 1200, bool enable_image_view = false);


#endif //PROCESSING_PIECE_SPLITTING_H
