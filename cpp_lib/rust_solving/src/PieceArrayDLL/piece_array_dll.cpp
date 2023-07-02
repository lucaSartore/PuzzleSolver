#include "piece_array_dll.h"
#include "../../../solving/puzzle_preview/PieceArray.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <filesystem>
using namespace std;

struct PieceArrayWrapper{
    static PieceImage* images;
    PieceArray pa;
};
PieceImage* PieceArrayWrapper::images = nullptr;

/// load all the piece image on the specified path
__declspec(dllexport) void load_images_to_piece_array_wrapper(const char* path){
    // count how many piece there are in the folder
    auto iter_files = std::filesystem::directory_iterator(path);
    int number_of_images = (int) count_if(
            begin(iter_files),
            end(iter_files),
            [](auto & entity){return true;}
    )/2; // divide by two because in the folder there are both .txt and .png files

    // create space for the images in memory
    PieceArrayWrapper::images = new PieceImage[number_of_images];

    // set the path
    PieceImage::set_origin_path(path);

    // fill up with images
    for(int i=0; i< number_of_images; i++){
        PieceArrayWrapper::images[i] = PieceImage(i);
    }
}

/// free the loaded images form the heep
__declspec(dllexport) void free_images_of_piece_array_wrapper(){
    delete[] PieceArrayWrapper::images;
    PieceArrayWrapper ::images = nullptr;
}

__declspec(dllexport) PieceArrayWrapper* create_piece_array(){
    auto pa = new PieceArrayWrapper();

    auto pieces_images = PieceArrayWrapper::images;
    Holder base = Holder(&pieces_images[4], 0);
    pa->pa.set(0,0,std::move(base));
    pa->pa.grow_x();

    base = Holder(&pieces_images[5], 3);
    pa->pa.set(1, 0, std::move(base));
    pa->pa.grow_y();

    base = Holder(&pieces_images[3], 3);
    pa->pa.set(0,1,std::move(base));


    base = Holder(&pieces_images[2], 0);
    pa->pa.set(1,1,std::move(base));

    return pa;
}

__declspec(dllexport) void generate_test_image(PieceArrayWrapper* piece_array_wrapper){
    auto img = piece_array_wrapper->pa.get_image(BuildImageMode::PREVIEW);
    cv::imwrite("test.png",img);
}

__declspec(dllexport) void destroy_piece_array_wrapper(PieceArrayWrapper* to_destroy){
    delete to_destroy;
}