//
// Created by luca on 5/2/23.
//

#ifndef PIECECLASS_IMPOSSIBLECOMBINATIONSGRAPH_H
#define PIECECLASS_IMPOSSIBLECOMBINATIONSGRAPH_H

#include "../../constants.h"
#include "GroupedPiecesNode.h"
#include <list>
#include <vector>
/*
 * the current program requires to know whether ot not two pieces have a sub component in common... this is because
 * if they have on the combination is impossible, since in a puzzle we only have one piece of each kind at a time
 *
 * do know this i have used a combination graph, in this graph the adjacent nodes are the one the current piece share
 * a border with, so i know that a combination is possible only if the 2 tested pieces do not have a connection between them
 * */
class ImpossibleCombinationsGraph {
private:
    // in the array there are MAX_GroupedPieces_LEVEL levels.
    // nodes[N] contains the connection for GroupedPieces<N+1>
    // the vector has a map 1:1 between his index and the id of GroupedPieces, as well as the index of GroupedPiecesHolder
    std::vector<GroupedPiecesNode> nodes[MAX_GroupedPieces_LEVEL];
public:
    /// default constructor, all pieces will be empty
    ImpossibleCombinationsGraph();
    /// initialize the vector or se specified level with a certen size.
    void create_nodes(int level, int size);
    /// insert an id on a specified level, you need to specify the ids of the 4 sub components (that will reference to the level: `level`-1)
    /// so that the adjacent connections could be calculated
    void insert_node(int level, int id, int id_sub_1, int id_sub_2, int id_sub_3, int id_sub_4);
    /// after all components has been placed you can use this function to calculate the adjacency of a certen leve
    void calculate_connections(int level);
};


#endif //PIECECLASS_IMPOSSIBLECOMBINATIONSGRAPH_H
