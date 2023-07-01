#include "communication_image.h"

#include <iostream>
#include <fstream>
#include <Windows.h>

PngImagePointer PngImagePointerConstructor(uchar* data_,unsigned long long len_){
    PngImagePointer to_return;
    to_return.data = data_;
    to_return.len = len_;
    return to_return;
}

/// construct an image
PngImageClass::PngImageClass(std::vector<uchar> && image_){
image = image_;
}
/// return a raw pointer to the data of the image
PngImagePointer PngImageClass::get_image_pointer(){
    return PngImagePointerConstructor(image.data(),image.size());
}

PngImageClass store_image_to_ram(const cv::Mat &image){
    // Encode the image as set of byte
    std::vector<uchar> encodedData;
    cv::imencode(".png", image, encodedData);
    return (std::move(encodedData));
}

cv::Mat load_image_from_ram(PngImagePointer png_image){
    // decoding the image from the bytes
    std::vector<byte> data = std::vector<byte>(png_image.len);
    std::memcpy(data.data(),png_image.data,png_image.len*sizeof(uchar));
    return cv::imdecode(data,cv::IMREAD_UNCHANGED);
}