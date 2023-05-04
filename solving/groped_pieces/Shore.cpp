//
// Created by luca on 4/21/23.
//

#include "Shore.h"

Shore::Shore() {
    shore = 1;
    number = 0;
}

Shore::Shore(float shore_) {
    shore = shore_;
    number = 1;
}

float Shore::get_shore()const{
    return shore;
}

void Shore::operator+=(const Shore &other) {
    int other_number = other.number;
    float other_shore = other.shore;

    // if both shore are zero return immediately to avoid zero division
    if(number == 0 && other_number == 0){
        return;
    }

    // a connection that is impossible (aka, shore is 0) has to remain impossible even of other
    // shore are grate
    if(shore == 0 || other_shore == 0){
        shore = 0;
        number = 1;
        return;
    }

    // calculate new shore whit weighted average
    shore = (shore*(float)number + other_shore*(float)other_number)/(float)(number+other_number);
    // sum numbers
    number+= other_number;

}

Shore Shore::operator+(const Shore &other) const {
    Shore r = *this;
    r+=other;
    return r;
}

std::ostream& operator<<(std::ostream &os, const Shore & s){
    return os << "shore: " << s.shore << " num: " << s.number;
}