//
// Created by luca on 5/1/23.
//

#ifndef PIECECLASS_UTIL_H
#define PIECECLASS_UTIL_H

#include <opencv2/opencv.hpp>

/// get a mat that is black on the border, and make it printable by cutting it down
void crop_image_to_remove_black_gb(cv::Mat& inputImage);

#endif //PIECECLASS_UTIL_H
