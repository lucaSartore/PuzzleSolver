#include "piece_array_dll.h"
#include "../../../solving/puzzle_preview/PieceArray.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <filesystem>
#include "../../../communication/communication_image.h"
#include "../../../solving/puzzle_preview/PreviewManager.h"
#include "../../../solving/util.h"
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

__declspec(dllexport) PieceArrayWrapper* create_piece_array_wrapper(uint64_t size_x, uint64_t size_y, SingePiece* pieces){
    // create the array
    auto pa = new PieceArrayWrapper();

    pa->pa = PieceArray(size_x,size_y);

    auto pieces_images = PieceArrayWrapper::images;

    // insert the image in the order
    for(uint64_t y=0; y<size_y; y++){

        for(uint64_t x=0; x<size_x; x++){
            uint64_t id = pieces->id;
            uint64_t orientation = pieces->orientation;

            pa->pa.set(x,y,std::move(Holder(pieces_images+id,orientation)));

            pieces++;
        }
    }

    return pa;
}

__declspec(dllexport) void generate_test_image(PieceArrayWrapper* piece_array_wrapper){
    auto img = piece_array_wrapper->pa.get_image(BuildImageMode::PREVIEW);
    cv::imwrite("test.png",img);
}

__declspec(dllexport) void destroy_piece_array_wrapper(PieceArrayWrapper* to_destroy){
    delete to_destroy;
}

/// return the shore (aka how well sed pieces fit together) of an image
__declspec(dllexport) float get_shore(PieceArrayWrapper* piece_array_wrapper){
    return piece_array_wrapper->pa.get_shore();
}

/// send a preview image to the c# app for the user to see
__declspec(dllexport) void generate_preview_image(PieceArrayWrapper* piece_array_wrapper){
    if(PreviewManager::is_preview_enabled()){
        cv::Mat image = piece_array_wrapper->pa.get_image(PREVIEW);
        crop_image_to_remove_black_gb(image);
        PreviewManager::output_preview_image(image);
    }
}