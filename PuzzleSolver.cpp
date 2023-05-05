//
// Created by luca on 5/3/23.
//
#include "PuzzleSolver.h"
#include <sys/stat.h>
using namespace std;
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "pre_processing/piece_splitting.h"
#include "pre_processing/corner_finding.h"
#include "solving/calculate_connections.h"
#include "solving/solve.h"
PuzzleSolver::PuzzleSolver(int dim_x, int dim_y, std::string work_path_, std::string origin_pat_,int number_of_cores_) {

    // constructing base parameters
    final_dim_x = dim_x;
    final_dim_y = dim_y;

    work_path = std::move(work_path_);
    origin_path = std::move(origin_pat_);

    number_of_cores = number_of_cores_;

    state = START;

    number_of_pieces = 0;

    // removing the working directory and all his sub componetns
    filesystem::remove_all(work_path);

    // creating the 4 folders used for save the data of the program
    string paths_to_create[] = {"", "/raw","/divided","/connections","/results"};
    for(const auto& to_create: paths_to_create) {
        string path = work_path + to_create;

        filesystem::create_directory(path);
    }

    // copying all the files contained in the input path inside the just created raw folder
    auto iter_files = std::filesystem::directory_iterator(origin_path);
    for (const auto & entry : iter_files){

        string new_path = work_path + "/raw/" + entry.path().filename().string();

        // make share the files are supported
        string extension = entry.path().filename().extension().string();

        // make shore the extension is right
        if(extension != ".png" && extension != ".jpeg" && extension != ".jpg"){
            throw invalid_argument("the origin path must contain only file with the supported format (png, jpeg, jpg)");
        }

        // copy the file
        std::filesystem::copy(entry,new_path);
    }

    save_status();
}

std::ostream& operator<<(std::ostream& os, const PuzzleSolver& ps) {
    os << "Final dimension: " << ps.final_dim_x << " x " << ps.final_dim_y << "\n";
    os << "Number of pieces: " << ps.number_of_pieces << "\n";
    os << "Work path: " << ps.work_path << "\n";
    os << "Origin path: " << ps.origin_path << "\n";
    os << "Number of cores: " << ps.number_of_cores << "\n";
    os << "State: " << ps.state << "\n";
    return os;
}

PuzzleSolver::PuzzleSolver(std::string work_path_) {
    load_status(work_path_ + "/status.txt");
}

void PuzzleSolver::split_image() {

    if(state != START){
        throw wrong_state_exception();
    }

    // split the images
    number_of_pieces = split_pieces_into_single_images(work_path + "/raw/", work_path + "/divided/");
    // update the state
    state = IMAGE_SPLITTED;
    // save the current status
    save_status();
}

void PuzzleSolver::save_status() {
    std::ofstream status_file;
    status_file.open(work_path + "/status.txt");
    if (status_file.is_open()) {
        status_file << "DO NOT EDIT THIS FILE MANUALLY" << "\n\n";
        status_file << final_dim_x << "\n";
        status_file << final_dim_y << "\n";
        status_file << number_of_pieces << "\n";
        status_file << work_path << "\n";
        status_file << origin_path << "\n";
        status_file << number_of_cores << "\n";
        status_file << static_cast<int>(state) << "\n";
        status_file.close();
    }else{
        throw file_system_exception();
    }
}

void PuzzleSolver::load_status(std::string file) {
    std::ifstream status_file;
    status_file.open(file);
    if (status_file.is_open()) {
        std::string line;
        // skip the 2 first lines
        std::getline(status_file, line);std::getline(status_file, line);

        std::getline(status_file, line);
        final_dim_x = std::stoi(line);

        std::getline(status_file, line);
        final_dim_y = std::stoi(line);

        std::getline(status_file, line);
        number_of_pieces = std::stoi(line);

        std::getline(status_file, line);
        work_path = line;

        std::getline(status_file, line);
        origin_path = line;

        std::getline(status_file, line);
        number_of_cores = std::stoi(line);

        std::getline(status_file, line);
        state = static_cast<State>(std::stoi(line));;

        status_file.close();
    }else{
        throw file_system_exception();
    }
}

void PuzzleSolver::process_corners() {
    if(state != IMAGE_SPLITTED){
        throw wrong_state_exception();
    }
    // find the corners
    find_corners(work_path + "/divided/",number_of_pieces,DEFAULT_PPI,number_of_cores);

    // update the state;
    state = CORNER_PROCESSED;

    save_status();
}

void PuzzleSolver::calculate_connections() {
    if(state != CORNER_PROCESSED){
        throw wrong_state_exception();
    }
    // calculate all combinations
    calculate_all_connections(work_path + "/divided/", work_path + "/connections", number_of_pieces,number_of_cores,false);
    // update state
    state = CONNECTION_CALCULATED;

    save_status();
}

void PuzzleSolver::solve_puzzle() {
    if(state != CONNECTION_CALCULATED){
        throw wrong_state_exception();
    }

    // find the solutions
    solve_puzzle_function(
            work_path+"/connections",
            work_path+"/divided",
            "",
            final_dim_x,
            final_dim_y,
            number_of_pieces,
            number_of_cores
            );

    //state = COMBINATION_CALCULATED;

    save_status();
}

std::ostream& operator<<(std::ostream& os, const State& state) {
    switch (state) {
        case State::START:
            os << "START";
            break;
        case State::IMAGE_SPLITTED:
            os << "IMAGE_SPLITTED";
            break;
        case State::CORNER_PROCESSED:
            os << "CORNER_PROCESSED";
            break;
        case State::CONNECTION_CALCULATED:
            os << "CONNECTION_CALCULATED";
            break;
        case State::COMBINATION_CALCULATED:
            os << "COMBINATION_CALCULATED";
            break;
        default:
            os << "INVALID_STATE";
            break;
    }
    return os;
}

void test_save_ad_load(){
    {
        // Create a new instance of PuzzleSolver
        PuzzleSolver ps(3, 55, "../work_path", "../dataset/test_4x4/raw");

        // Print the first instance
        std::cout << "First instance:\n" << ps << "\n";
    } // now the ps instance is out of scope and gets deallocated

    // Create a new instance of PuzzleSolver using the work_path from the previous instance
    PuzzleSolver ps2("../work_path");

    // Print the second instance
    std::cout << "Second instance:\n" << ps2 << "\n";

}