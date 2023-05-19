//
// Created by lucas on 18/05/2023.
//

#ifndef PUZZLESOLVER_PREVIEWMANAGER_H
#define PUZZLESOLVER_PREVIEWMANAGER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <condition_variable>



/// this class is intended to be used statically
/// it is use to show many image even in a multithreaded environment
class PreviewManager {
private:
    // whether the preview is enabled or not
    static bool preview_enable;
    // file where to store the preview image
    static std::string output_file;
    // is true if the data is ready to be read, false otherwise
    static bool ready_to_read;
    // mutex and condition for synchronization
    static std::condition_variable cond_v;
    static std::mutex mtx;
    // flag that is true only the first time an image is read
    static bool is_first_read;

public:

    /// wait util a program update the preview imag
    /// if you wait more than the max wait time the function throws an error
    /// if preview is disable this immediately return and give a warning
    /// returns true if he has written an image
    /// return false is the max waiting time has been exceeded, or if debug is set to false
    static bool next_preview_image(float max_waiting_time = 10);

    /// pause the current program until someone calls get_next_preview_image, then it save the image in output file and
    /// continue the execution
    /// dose not block the program if preview is disable;
    static void output_preview_image(cv::Mat & image);

    /// enable the preview
    static void enable_preview();

    /// disable the preview, and also unlock all threads that are waiting in output_preview_image
    static void disable_preview();

    /// check if preview is enabled or not
    static bool is_preview_enabled();

    /// set the file where the program will output the preview images
    static void set_output_file(const char* new_file);

    /// return the file of the preview image that will be created
    static std::string get_output_file();
};


#endif //PUZZLESOLVER_PREVIEWMANAGER_H
