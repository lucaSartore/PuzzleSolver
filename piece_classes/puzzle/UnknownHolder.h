//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_UNKNOWNHOLDER_H
#define PIECECLASS_UNKNOWNHOLDER_H

#include "Holder.h"

class UnknownHolder: public Holder{
public:
    UnknownHolder() = default;
    virtual ~UnknownHolder() = default;
    virtual char get_debug_view() const;
};


#endif //PIECECLASS_UNKNOWNHOLDER_H
