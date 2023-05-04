//
// Created by luca on 5/3/23.
//
#include "PuzzleSolver.h"
#include <sys/stat.h>
using namespace std;
#include <fstream>


void PuzzleSolver::save_status() {
    std::ofstream status_file;
    status_file.open(work_path + "/status.txt");
    if (status_file.is_open()) {
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
        std::getline(status_file, line);
        final_dim_x = std::stoi(line);
        std::getline(status_file, line);
        final_dim_y = std::stoi(line);
        std::getline(status_file, line);
        number_of_pieces = std::stoi(line);
        std::getline(status_file, line);
        origin_path = line;
        std::getline(status_file, line);
        work_path = line;
        std::getline(status_file, line);
        number_of_cores = std::stoi(line);
        std::getline(status_file, line);
        state = static_cast<State>(std::stoi(line));
        status_file.close();
    }else{
        throw file_system_exception();
    }
}
PuzzleSolver::PuzzleSolver(int dim_x, int dim_y, std::string work_path_, std::string origin_pat_,int number_of_cores_) {

    // constructing base parameters
    final_dim_x = dim_x;
    final_dim_y = dim_y;

    work_path = std::move(work_path_);
    origin_path = std::move(origin_pat_);

    number_of_cores = number_of_cores_;

    state = START;

    // creating the 4 folders used for save the data of the program
    string paths_to_create[] = {"", "/raw","/divided","/connections","/results"};
    for(const auto& to_create: paths_to_create) {
        string path = work_path + to_create;

        int dir_err = mkdir(path.c_str(), S_IRWXU | S_IROTH);
        if (dir_err == -1) {
            throw file_system_exception();
        }
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