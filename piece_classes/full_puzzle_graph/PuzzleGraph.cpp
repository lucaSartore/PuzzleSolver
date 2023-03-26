//
// Created by luca on 3/23/23.
//

#include "PuzzleGraph.h"
#include <filesystem>
#include <algorithm>

using namespace std;

PuzzleGraph::PuzzleGraph(const std::string& path_to_connection_data) {

    auto iter_files = std::filesystem::directory_iterator(path_to_connection_data);

    number_of_pieces = (int) std::count_if(
            begin(iter_files),
            end(iter_files),
            [](auto & entity){return true;}
    );

    pieces = new PieceNode[number_of_pieces];

    for(int i=0; i<number_of_pieces; i++){

        PieceConnections piece = PieceConnections(path_to_connection_data,i);

        pieces[i] = PieceNode(piece, pieces);

    }


}

PuzzleGraph::~PuzzleGraph() {
    delete[] pieces;
}
