//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PREVIEWHOLDER_H
#define PIECECLASS_PREVIEWHOLDER_H

#include "PieceImage.h"
#include <opencv2/opencv.hpp>
#include "../groped_pieces/Direction.h"
#include "Holder.h"


class PreviewHolder: public Holder{
private:
    cv::Scalar color;
public:
    /// set the color of the current piece;
    void set_color(cv::Scalar new_color);
    /// return the current color of the piece;
    cv::Scalar get_color();
    /// zero parameter constrictor
    PreviewHolder();
    /// constructor
    PreviewHolder(PieceImage *piece_, int orientation_);
    /// destructor
    ~PreviewHolder() = default;
};


#endif //PIECECLASS_PREVIEWHOLDER_H