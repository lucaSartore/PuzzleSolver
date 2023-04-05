//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_PIECEHOLDER_H
#define PIECECLASS_PIECEHOLDER_H

#include "Holder.h"

class PieceHolder: public Holder{
public:
    PieceHolder() = default;
    virtual ~PieceHolder() = default;
    virtual char get_debug_view() const;
};


#endif //PIECECLASS_PIECEHOLDER_H
