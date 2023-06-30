#include "piece_array_dll.h"
#include "../../../solving/puzzle_preview/PieceArray.h"
#include "opencv2/opencv.hpp"


struct PieceArrayWrapper{
    PieceImage* images
    PieceArray pa;
};

PieceArrayWrapper* create_piece_array(){
    return new PieceArrayWrapper();
}

void generate_test_image(PieceArrayWrapper* piece_array_wrapper){

    Holder o = Holder()

    piece_array_wrapper->pa.set(x,y,)

    auto img = piece_array_wrapper->pa.get_image(BuildImageMode::PREVIEW);
    cv::imwrite("test.png",img);
}

void destroy_piece_array_wrapper(PieceArrayWrapper* to_destroy){
    delete to_destroy;
}