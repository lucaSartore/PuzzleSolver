//
// Created by luca on 4/5/23.
//

#include "OutsideHolder.h"

char OutsideHolder::get_debug_view() const {
    return 'O';
}

bool OutsideHolder::is_outside() {
    return true;
}

OutsideHolder::OutsideHolder(): Holder() {}

OutsideHolder::~OutsideHolder(){}

