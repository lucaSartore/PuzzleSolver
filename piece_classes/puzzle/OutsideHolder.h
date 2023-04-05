//
// Created by luca on 4/5/23.
//

#ifndef PIECECLASS_OUTSIDEHOLDER_H
#define PIECECLASS_OUTSIDEHOLDER_H

#include "Holder.h"

class OutsideHolder: public Holder{
public:
    char get_debug_view() const override;
    /// say if this holder is an outside holder or not
    virtual bool is_outside();
    OutsideHolder();
};


#endif //PIECECLASS_OUTSIDEHOLDER_H
