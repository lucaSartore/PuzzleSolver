//
// Created by luca on 4/4/23.
//

#include "Puzzle.h"


#include <utility>
#include "PieceHolder.h"
#include <memory>
#include <algorithm>

using namespace std;

Puzzle::Puzzle(std::string path, int number_of_pieces_) {

    array = PieceArray();

    // alloc space for the pieces
    pieces = new Piece[number_of_pieces_];

    // set the number of pieces
    number_of_pieces = number_of_pieces_;

    // load all the pieces one by one
    Piece::set_origin_path(std::move(path));
    for(int i=0; i<number_of_pieces_;i++){
        pieces[i] = Piece(i);
    }

    min_compatibility = DEFAULT_MIN_COMPATIBILITY;

    // inserting the pieces in the available_pieces
    for(int i=0; i<number_of_pieces; i++){
        available_pieces.push_back(pieces+i);
    }

    // create the pointer
    puzzle_pointer = PuzzlePointer(&array);
}

Puzzle::~Puzzle() {
    // free all the pieces from ram;
    delete[] pieces;
}

void Puzzle::set_min_compatibility(float new_value) {
    min_compatibility = new_value;
}

std::list<std::tuple<float,shared_ptr<Holder>>> Puzzle::get_best_fits(int x, int y) {
    std::list<std::tuple<float,shared_ptr<Holder>>> best_fits;

    // Iterate through all the available pieces
    for (auto piece : available_pieces) {
        for(int orientation = 0; orientation<4; orientation++){

            shared_ptr<Holder>  to_test = shared_ptr<Holder>(new PieceHolder(piece,orientation));

            float compatibility = array.check_compatibility(x,y,to_test.get());

            // Only consider pieces with a compatibility score greater than min_compatibility
            if (compatibility >= min_compatibility) {
                // create the tuple
                auto to_insert = tuple<float, shared_ptr<Holder>>(compatibility,to_test);
                // Add the compatibility score and the piece to the list_x_y of best fits
                best_fits.emplace_back(to_insert);
            }
        }
    }

    // Sort the list_x_y of best fits by compatibility score in descending order

    best_fits.sort([]( std::tuple<float, std::shared_ptr<Holder>>& a,  std::tuple<float, std::shared_ptr<Holder>>& b) {
        return std::get<0>(a) > std::get<0>(b);
    });

    // Truncate the list_x_y to contain only the top 10 best fits
    if (best_fits.size() > 10) {
        best_fits.resize(10);
    }

    return best_fits;
}


bool Puzzle::solve() {



    // growing the array to allow search
    array.grow_y();
    array.grow_x();
    array.grow_y();
    array.grow_x();
    array.grow_y();
    array.grow_x();
    array.grow_y();
    array.grow_x();
    array.grow_y();
    array.grow_x();




    // find one of the corners and put it in place
    auto bests = get_best_fits(0,0);
    tuple<float,shared_ptr<Holder>> best_element = bests.front();
    auto corner =  std::get<1>(best_element);
    auto corner_upcast = std::static_pointer_cast<PieceHolder>(corner);

    // putting the corner in place
    array.set(0,0,corner);

    // removing the corner for the searchable pieces
    available_pieces.remove(corner_upcast->get_piece());

    // increasing the puzzle pointer by one
    puzzle_pointer.next();

    show();

    return solve_puzzle_recursive(*this);
}

void Puzzle::show() {
    cv::Mat img = array.get_image();
    cv::Mat resized;
    resize(img,resized,cv::Size(950,950));
    imshow("puzzle",resized);
    cv::waitKey(1);
}

// as convention when i call the function puzzle pointer need to point to the first empty element that needs to be fit
bool Puzzle::solve_puzzle_recursive(Puzzle &puzzle) {

    cout << "Got call with pointer at: " << puzzle.puzzle_pointer << endl;

    // calculate where i'm in the puzzle
    int x = puzzle.puzzle_pointer.get_x();
    int y = puzzle.puzzle_pointer.get_y();

    // find the best fits to the current piece
    auto bests = puzzle.get_best_fits(x,y);


    cout << "fits are empty conditions: " << bests.empty() << endl;

    // check all the elements one by one
    for(auto best: bests){
        auto best_ptr = std::get<1>(best);
        auto best_ptr_upcast = static_pointer_cast<PieceHolder>(best_ptr);

        // insert the candidate piece in the puzzle to test it
        puzzle.array.set(x,y,best_ptr);

        // show the choice for debug
        puzzle.show();

        // removing the candidate piece form the pieces that are available to solve the puzzle
        puzzle.available_pieces.remove(best_ptr_upcast->get_piece());

        // go one element further, if it return false it means that it can no longer go on, because it has reached the end,
        // witch means the puzzle has been solved
        if(!puzzle.puzzle_pointer.next()){
            return true;
        }

        // now i call the inductive case, and try to solve tne new puzzle, if this seed i fund the solution, and return true
        if(solve_puzzle_recursive(puzzle)){
            return true;
        }

        // if the solution has not been solved i need to go back so:

        // i insert the puzzle back to available pieces
        puzzle.available_pieces.push_back(best_ptr_upcast->get_piece());

        // i roll back the pointer
        puzzle.puzzle_pointer.prev();

        // i remove the piece from the array
        puzzle.array.remove(x,y);
    }

    // if none of the tested possibility where ok, it means that the puzzle was not possible to solve in this configuration,
    // so i return false
    return false;
}


PuzzlePointer::PuzzlePointer(PieceArray *reference_array_) {
    list_x_y = std::list<std::tuple<int,int>>();
    reference_array = reference_array_;
    x = 0;
    y = 0;
}

PuzzlePointer::PuzzlePointer() {
    reference_array = nullptr;
    x = 0;
    y = 0;
}

int PuzzlePointer::get_x() const {
    return x;
}

int PuzzlePointer::get_y() const {
    return y;
}

bool PuzzlePointer::next() {
    // getting the current pointed piece
    Holder* pointed_piece = reference_array->get(x,y);
    // need to point to a piece in order for it to be valid
    if(!pointed_piece->is_a_piece()){
        throw runtime_error("unable to determine the next piece do to unknown disposition of array | the current pointed piece is unknown");
    }

    // the pieces to the right and left must not be unknown (can only be outside or unknown)
    if(reference_array->get(x-1,y)->is_unknown()){
        throw runtime_error("unable to determine the next piece do to unknown disposition of array | piece to the left is unknown");
    }
    if(reference_array->get(x,y-1)->is_unknown()){
        throw runtime_error("unable to determine the next piece do to unknown disposition of array | piece above is unknown");
    }


    // at this point i check if i am at the bottom right corner (aka the puzzle is finished) and return false in case
    if(reference_array->get(x+1,y)->is_outside() && reference_array->get(x,y+1)->is_outside()){
        return false;
    }// in all other case i will return true


    // push element to the list so i remember my way back
    list_x_y.emplace_back(x,y);

    // now i can be in one of 3 cases:

    // case 1: i am in in the Y axes that has been filled
    if(reference_array->get(x-1,y+1)->is_a_piece()){
        cout << "next case: 1" << endl;
        y++;
        return true;

    }
    // case 2: i am in in the X axes that has been filled
    else if(reference_array->get(x+1,y-1)->is_a_piece()){
        cout << "next case: 2" << endl;
        x++;
        return true;
    }
    // case 3: row x and y has finished filling, and i need to start a new row/colon
    else{
        // 3.1 start a new colon
        if(y>x || reference_array->get(x,y+1)->is_outside()){
            cout << "next case: 3.1" << endl;
            x++;
            y=0;
            return true;
        }
        // 3.2 start a new row
        else{
            cout << "next case: 3.2" << endl;
            x=0;
            y++;
            return true;
        }
    }

}

bool PuzzlePointer::prev() {

    // if the list is false it means the puzzle is at is initial state
    if(list_x_y.empty()){
        return false;
    }

    auto tuple = list_x_y.back();
    list_x_y.pop_back();

    x = std::get<0>(tuple);
    y = std::get<1>(tuple);

    return true;
}

ostream & operator<<(ostream& os, const PuzzlePointer & pp){
    return os << "(" << pp.get_x() << "," << pp.get_y() << ")";
}