//
// Created by luca on 3/23/23.
//

#ifndef PIECECLASS_PUZZLEGRAPH_H
#define PIECECLASS_PUZZLEGRAPH_H


#include <iostream>
#include "PieceNode.h"

class PuzzleGraph {
public:
    /// the number of pieces in this graph
    int number_of_pieces;
    /// array containing all the pieces
    PieceNode * pieces;
    /// construct a puzzle graph, need to specify where the data are saved
    explicit PuzzleGraph(const std::string& path_to_connection_data);
    /// use an algorithm of adjacent pieces to identify if some connections needs to be removed
    /// it return the number of pieces that it has removed
    int exclude_some_connections(bool use_multi_threading = false);
    /// run an algorithm that find the distance form each connected piece following right or left direction
    void calculate_distances(bool use_multi_threading = false);
    /// reset all the data about distances
    void reset_distances();
    ~PuzzleGraph();
};


#endif //PIECECLASS_PUZZLEGRAPH_H
