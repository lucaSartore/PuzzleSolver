//
// Created by luca on 4/4/23.
//

#include "Holder.h"

char Holder::get_debug_view() const{
    return 'H';
}

bool Holder::is_unknown() {
    return false;
}

float Holder::check_compatibility(Holder *up, Holder *down, Holder *left, Holder *right) {
    return 0;
}

Side *Holder::get_side(Direction direction) {
    return nullptr;
}

bool Holder::is_outside() {
    return false;
}
