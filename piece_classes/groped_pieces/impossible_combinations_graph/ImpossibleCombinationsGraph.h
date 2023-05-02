//
// Created by luca on 5/2/23.
//

#ifndef PIECECLASS_IMPOSSIBLECOMBINATIONSGRAPH_H
#define PIECECLASS_IMPOSSIBLECOMBINATIONSGRAPH_H

#include "../GroupedPieces.h"
#include "GroupedPiecesNode.h"
#include "list"
/*
 * the current program requires to know whether ot not two pieces have a sub component in common... this is because
 * if they have on the combination is impossible, since in a puzzle we only have one piece of each kind at a time
 *
 * do know this i have used a combination graph, in this graph the adjacent nodes are the one the current piece share
 * a border with, so i know that a combination is possible only if the 2 tested pieces do not have a connection between them
 * */
class ImpossibleCombinationsGraph {
    // in the array there are MAX_GroupedPieces_LEVEL levels, each level
    std::list<GroupedPiecesNode> nodes[MAX_GroupedPieces_LEVEL];
};


#endif //PIECECLASS_IMPOSSIBLECOMBINATIONSGRAPH_H
