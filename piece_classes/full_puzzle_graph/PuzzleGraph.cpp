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

// each piece need to know what pieces are reachable ad a distance n (0<=n<=2) by only
// turning right or left, this is useful to delete some of the connections
void PuzzleGraph::calculate_distances(bool use_multi_threading) {

    // need to calculate each distance one by one
    for(int distance=0; distance<2; distance++){
        // analyzing pieces one by one
        for(int i=0; i<number_of_pieces; i++){
            cout << "distance: " << distance << "/2" << endl << "pieces: " << i << "/" << number_of_pieces << endl;
            // analyzing sides one by one
            for(int side=0; side<4; side++){
                // get a pointer to the side of the current piece
                SideNode *side_this_piece = &pieces[i].get_side(side);

                // get the nodes that right now are reachable ad the left and at the right
                set<SideNode*> close_sides_left = side_this_piece->get_reachable_pieces(distance,LEFT);
                set<SideNode*> close_sides_right = side_this_piece->get_reachable_pieces(distance,RIGHT);

                // calculating the pieces reachable at a distance of `distance + 1` in the left direction
                for(auto close_side: close_sides_left){
                    // now i get the side to the left of the current side, and get the pieces clse to it
                    // note that since the distance is 0 it doe not matter whether the direction is left
                    // or right in the `get_reachable_pieces` function
                    set<SideNode*> new_close_pieces = close_side->get_side_to(LEFT).get_reachable_pieces(0,LEFT);
                    // now that i have the new side i can add them in the distance+1 section, so that the next iteration they will be there
                    side_this_piece->insert_as_reachable(new_close_pieces,distance+1,LEFT);
                }

                // doing the same thing, but for the right direction
                for(auto close_side: close_sides_right){
                    set<SideNode*> new_close_pieces = close_side->get_side_to(RIGHT).get_reachable_pieces(0,RIGHT);
                    side_this_piece->insert_as_reachable(new_close_pieces,distance+1,RIGHT);
                }
            }
        }
    }
}


int PuzzleGraph::exclude_some_connections(bool use_multi_threading) {

    int excluded = 0;

    for(int i=0; i<number_of_pieces; i++){
        for(int side=0; side<4; side++){
            // the side i am analyzing
            SideNode* tested_side = &pieces[i].get_side(side);
            // the side i need to connect to in order to verify a connection
            SideNode* expected_side_to_reach = &tested_side->get_side_to(RIGHT);
            // i can create a connection only if the other piece is not a side
            if(!expected_side_to_reach->get_reachable_pieces(0, LEFT).empty()){

                set<SideNode*> to_remove = set<SideNode*>();

                // need to test each connection individually
                for(auto connection_to_test: tested_side->get_reachable_pieces(0,LEFT)){

                    // if the side is not reachable, than i save it to be removed
                    if(!connection_to_test->get_side_to(LEFT).is_reachable(expected_side_to_reach,2,LEFT)){
                        to_remove.insert(connection_to_test);
                    }
                }
                for(auto to_remove_: to_remove){
                    tested_side->remove_connection(to_remove_);
                    excluded++;
                }
            }
        }
    }
    return excluded;
}
