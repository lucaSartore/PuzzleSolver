//
// Created by luca on 3/23/23.
//

#ifndef PIECECLASS_PUZZLEGRAPH_H
#define PIECECLASS_PUZZLEGRAPH_H


#include <iostream>
#include "PieceNode.h"

class PuzzleGraph {
public:
    int number_of_pieces;
    PieceNode * pieces;
    explicit PuzzleGraph(const std::string& path_to_connection_data);
    ~PuzzleGraph();
};


#endif //PIECECLASS_PUZZLEGRAPH_H
