//
// Created by luca on 3/16/23.
//

#ifndef PROCESSING_PIECE_SPLITTING_H
#define PROCESSING_PIECE_SPLITTING_H

#include <opencv2/opencv.hpp>
#include <iostream>


/// this function take as input a input_path where some scansion of a puzzle_preview is made
/// and split them into many single pieces
int split_pieces_into_single_images(const std::string& input_path,const std::string& output_path, const std::string& result_path, int ppi = 1200, bool enable_image_view = false);

/// set the threshold for image splitting
void piece_splitting_set_threshold(unsigned char new_threshold);

/// get a threshold image to test if the threshold value si good
cv::Mat piece_splitting_get_test_threshold_image(const std::string& input_path);
#endif //PROCESSING_PIECE_SPLITTING_H
