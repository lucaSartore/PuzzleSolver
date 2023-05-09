//
// Created by luca on 5/5/23.
//

#ifndef PIECECLASS_CALCULATE_CONNECTIONS_H
#define PIECECLASS_CALCULATE_CONNECTIONS_H

#include <iostream>


/// take the image and metadata about corners saved in `input path`, then elaborate all possible connections and save
/// them in output path.
/// this function allows the user to specify the number of cores to use, if is 0 there will be an automatic detection.
/// it si possible to use the debug flag, ONLY if number of cores is 1
void calculate_all_connections(std::string input_path, std::string output_path,unsigned int number_of_pieces, unsigned int number_of_cores = 1, bool debug = false);


#endif //PIECECLASS_CALCULATE_CONNECTIONS_H
