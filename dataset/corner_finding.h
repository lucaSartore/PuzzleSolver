#ifndef PROCESSING_CORNER_FINDING_H
#define PROCESSING_CORNER_FINDING_H

#define DEFAULT_PPI 1200

#include <iostream>
#include <opencv2/opencv.hpp>


/// this function read all the files in input path, (the file has to be generated by the `split_pieces_into_single_images` function
/// and you may not change the name or extension!
/// it then save a txt file with the coordinates of the pieces in the same directory
void do_pre_processing(const std::string& path, int number_of_pieces, int ppi = DEFAULT_PPI, bool use_multithreading = false, bool enable_image_view = false);

void do_pre_processing_thread(const std::string& path, int piece_index, int ppi = DEFAULT_PPI, bool enable_image_view = false);
void remove_holes(const cv::Mat &input, cv::Mat &output,int ppi = DEFAULT_PPI);
void remove_knobs(const cv::Mat &input, cv::Mat &output, int ppi = DEFAULT_PPI);
void find_corners(const cv::Mat &input, cv::Point &p1,  cv::Point &p2,  cv::Point &p3,  cv::Point &p4, int ppi = DEFAULT_PPI);


#endif //PROCESSING_CORNER_FINDING_H