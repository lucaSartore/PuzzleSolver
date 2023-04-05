//
// Created by luca on 4/4/23.
//

#ifndef PIECECLASS_HOLDER_H
#define PIECECLASS_HOLDER_H


class Holder {
public:
    Holder() = default;
    virtual ~Holder() = default;
    virtual char get_debug_view() const;
};


#endif //PIECECLASS_HOLDER_H
