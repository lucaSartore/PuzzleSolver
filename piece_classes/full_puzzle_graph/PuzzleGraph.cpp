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

            // keeping the sides that are not connectable, and so they will be removed
            set<SideNode*> to_remove = set<SideNode*>();

            // the side i need to connect to in order to verify a connection, by turning left and right
            SideNode* expected_side_to_reach_turning_left = &tested_side->get_side_to(RIGHT);
            SideNode* expected_side_to_reach_turning_right = &tested_side->get_side_to(LEFT);


            // need to test each connection individually, this are the pieces that connect th the current piece,
            // the idea here is that if you can't go back to the original piece from here tha side si wrong
            for(auto possible_connection: tested_side->get_reachable_pieces(0, LEFT)){

                // left turn (or counter clock wise)
                // if the piece i want to reach has 0 connection is probably a side, and is impossible to compleate
                // the circle
                if(!expected_side_to_reach_turning_left->get_reachable_pieces(0,LEFT).empty()){
                    // if the side is not reachable, than i save it to be removed
                    if(!possible_connection->get_side_to(LEFT).is_reachable(expected_side_to_reach_turning_left, 2, LEFT)){
                        to_remove.insert(possible_connection);
                    }
                }
                // right turn (or clock wise)
                if(!expected_side_to_reach_turning_right->get_reachable_pieces(0,LEFT).empty()){
                    // if the side is not reachable, than i save it to be removed
                    if(!possible_connection->get_side_to(RIGHT).is_reachable(expected_side_to_reach_turning_right, 2, RIGHT)){
                        to_remove.insert(possible_connection);
                    }
                }

            }

            // removing the connection that are not verified!
            for(auto to_remove_: to_remove){
                //to_remove_->remove_connection(tested_side);
                tested_side->remove_connection(to_remove_);
                excluded++;
            }

        }
    }
    return excluded;
}

void PuzzleGraph::reset_distances() {
    for(int i=0; i<number_of_pieces; i++){
        pieces[i].reset_distances();
    }
}
