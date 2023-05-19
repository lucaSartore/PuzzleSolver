//
// Created by lucas on 18/05/2023.
//
#include "PreviewManager.h"

bool PreviewManager::preview_enable = false;
std::string PreviewManager::output_file;
bool PreviewManager::ready_to_read = false;
std::condition_variable PreviewManager::cond_v;
std::mutex PreviewManager::mtx;

bool PreviewManager::is_preview_enabled() {
    return preview_enable;
}

void PreviewManager::next_preview_image(float max_waiting_time) {
    if(!preview_enable){
        // throw warning
        std::cerr << "Warning: you have called the \"next_preview_image\" function, but preview is disabled" << std::endl;
        return;
    }

    // send signal that I need a new data
    mtx.lock();
    ready_to_read = false;
    mtx.unlock();

    // notify one
    cond_v.notify_one();

    // wait until the data can be red am ready to read
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(!ready_to_read){
            cond_v.wait((lock), [] {return ready_to_read;});
        }
    }
}

void PreviewManager::output_preview_image(cv::Mat &image) {

    // if the preview is disabled no need to wait
    if(!preview_enable){
        return;
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        // if the data is no longer ready and can be rewritten
        if (ready_to_read) {
            cond_v.wait(lock, [] { return !ready_to_read; });
        }
        // write the image
        cv::imwrite(output_file,image);

        // say that the data is ready
        ready_to_read = true;
    }
    // notify all (to make sure I notify the consumer)
    cond_v.notify_all();
}


void PreviewManager::enable_preview() {
    preview_enable = true;

    // reset the default state
    mtx.lock();
    ready_to_read = false;
    mtx.unlock();
}

void PreviewManager::set_output_file(std::string &new_file) {
    output_file = new_file;
}

std::string PreviewManager::get_output_file() {
    return output_file;
}

void PreviewManager::disable_preview() {
    preview_enable = false;
    ready_to_read = true;

    // doto: unlock potential thread that might be waiting
}
