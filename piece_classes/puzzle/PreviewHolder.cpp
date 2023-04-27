//
// Created by luca on 4/4/23.
//

#include "PreviewHolder.h"

using namespace cv;


PreviewHolder::PreviewHolder(PieceImage *piece_, int orientation_): Holder(piece_,orientation_){
    color = cv::Scalar(255,255,255);
}

void PreviewHolder::set_color(cv::Scalar new_color) {
    color = std::move(new_color);
}

cv::Scalar PreviewHolder::get_color() {
    return color;
}

PreviewHolder::PreviewHolder(): Holder() {
    color = Scalar(255,255,255);
}

