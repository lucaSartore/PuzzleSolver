//
// Created by lucas on 18/05/2023.
//
#include "PreviewManager.h"

bool PreviewManager::preview_enable = false;
bool PreviewManager::ready_to_read = false;
std::condition_variable PreviewManager::cond_v;
std::mutex PreviewManager::mtx;
bool PreviewManager::is_first_read = true;
PngImageClass PreviewManager::image_in_ram = PngImageClass(std::vector<uchar>());

bool PreviewManager::is_preview_enabled() {
    return preview_enable;
}

bool PreviewManager::next_preview_image(float max_waiting_time) {
    if(!preview_enable){
        // throw warning
        std::cerr << "Warning: you have called the \"next_preview_image\" function, but preview is disabled" << std::endl;
        return false;
    }

    if(is_first_read) {
        is_first_read = false;
    }else{
        // send signal that I need a new data
        mtx.lock();
        ready_to_read = false;
        mtx.unlock();

        // notify one
        cond_v.notify_one();
    }

    // wait until the data can be red am ready to read
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(!ready_to_read){
            return cond_v.wait_for(lock,std::chrono::duration<float>(max_waiting_time), [] {return ready_to_read;});
        }
    }
    // if I haven't waited the image is ready
    return true;
}

void PreviewManager::output_preview_image(cv::Mat &image) {

    // if the preview is disabled no need to wait
    if(!preview_enable){
        return;
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        // wait until the data is no longer ready
        if (ready_to_read) {
            cond_v.wait(lock, [] { return (!ready_to_read) || !preview_enable; });
        }
        if(!preview_enable){
            return;
        }
        // write the image to ram
        image_in_ram = store_image_to_ram(image);

        // say that the data is ready
        ready_to_read = true;
    }
    // notify all (to make sure I notify the consumer)
    cond_v.notify_all();
}


void PreviewManager::enable_preview() {
    // reset the default state
    mtx.lock();
    ready_to_read = false;
    preview_enable = true;
    mtx.unlock();
}

void PreviewManager::disable_preview() {
    mtx.lock();
    preview_enable = false;
    ready_to_read = true;
    is_first_read = true;
    mtx.unlock();
    cond_v.notify_all();
    // doto: unlock potential thread that might be waiting
}

PngImagePointer PreviewManager::get_image_binary() {
    return image_in_ram.get_image_pointer();
}

cv::Mat PreviewManager::get_image() {
    return load_image_from_ram(get_image_binary());
}
