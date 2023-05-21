#ifndef PUZZLESOLVER_COMUNICATION_IMAGE_H
#define PUZZLESOLVER_COMUNICATION_IMAGE_H

#include "opencv2/opencv.hpp"

/*
 * this file is the main point of communication between the front end in c# and the back and in c++
 * */

// contain a raw pointer and the len information of an array
struct PngImagePointer{
    uchar* data;
    unsigned long long len ;
    PngImagePointer(uchar* data_,unsigned long long len_);
};

class PngImageClass{
private:
    std::vector<uchar> image;
public:
    /// construct an image
    PngImageClass(std::vector<uchar> && image_);
    /// default consort
    PngImageClass()=default;
    /// return a raw pointer to the data of the image
    PngImagePointer get_image_pointer();
};

/// write the image in ram so a new program could read it
PngImageClass store_image_to_ram(const cv::Mat &image);

/// takes the binary data inside the image, and loan a mat from them
cv::Mat load_image_from_ram(PngImagePointer png_image);

#endif //PUZZLESOLVER_COMUNICATION_H