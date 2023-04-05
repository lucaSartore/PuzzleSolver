//
// Created by luca on 4/4/23.
//

#include "PieceArray.h"
#include "stdexcept"
#include "UnknownHolder.h"
using namespace std;


PieceArray::PieceArray() {
    dim_x = 1;
    dim_y = 1;
    pieces = vector<vector<Holder*>>();

    // start with a 1*1 array with a null ptr inside
    auto one_dim = vector<Holder*>();
    one_dim.push_back(new UnknownHolder());
    pieces.push_back(one_dim);
}

const Holder &PieceArray::get(int x, int y) const{
    // check if in range
    check_indexes(x,y);

    // get the value
    Holder* to_return = pieces[x][y];
    if(to_return == nullptr){
        cerr << "index at X: " << x << " y: " << y << " is null ptr" << endl;
        throw std::runtime_error("specified index is null ptr");
    }
    return *to_return;
}

void PieceArray::set(int x, int y, Holder &to_be_set) {
    // check if in range
    check_indexes(x,y);
    pieces[x][y] = new Holder(to_be_set);
}

void PieceArray::remove(int x, int y) {
    // check if in range
    check_indexes(x,y);

    // get the value to remove
    Holder* to_remove = pieces[x][y];

    // free the memory
    delete to_remove;

    // insert a new unknown value
    pieces[x][y] = new UnknownHolder();
}

void PieceArray::check_indexes(int x, int y) const {
    // check if indexes are in range
    if(x<0 || y<0 || x >= dim_x || y >= dim_y){
        cerr << "index at X: " << x << " y: " << y << " is out of range for array with size: X: " << dim_x << " Y: " << dim_y << endl;
        throw std::invalid_argument("index out of range");
    }
}

void PieceArray::grow_x() {
    auto new_colon = vector<Holder*>();
    for(int i=0; i<dim_y; i++){
        new_colon.push_back(new UnknownHolder());
    }
    pieces.push_back(new_colon);
    dim_x++;
}

void PieceArray::grow_y() {
    for(int i=0; i<dim_x; i++){
        pieces[i].push_back(new UnknownHolder());
    }
    dim_y++;
}

int PieceArray::get_dim_x() const{
    return dim_x;
}

int PieceArray::get_dim_y() const  {
    return dim_y;
}

std::ostream& operator<<(std::ostream& os, const PieceArray& pa){
    int dim_x = pa.get_dim_x();
    int dim_y = pa.get_dim_y();

    for(int y = dim_y-1; y>=0; y--){
        for(int x = 0; x<dim_x; x++){
            os << pa.get(x,y).get_debug_view() << " ";
        }
        os << endl;
    }
    return os;
}

