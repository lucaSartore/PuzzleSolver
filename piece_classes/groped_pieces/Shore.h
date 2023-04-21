//
// Created by luca on 4/21/23.
//

#ifndef PIECECLASS_SHORE_H
#define PIECECLASS_SHORE_H


/// this class keeps track of the shore of each possible combination of pieces,
/// need to keep track of the average shore, and the number of pieces the shore keeps track of
/// dis is used to make a weighted average when combining multiple Shores
class Shore {
private:
    /// the average shore of the n represented connections
    float shore;
    /// the number of connections this shore is representing
    int number;
public:
    /// default constructor, gives a shore with `shore` = 1 and `number` = 0
    Shore();
    /// create a new shore, `number` will be 1
    Shore(float shore_);
    /// get the shore of the piece
    float get_shore()const;
    /// sum operator, this make a weighted of `shore`, using `number` as weight
    void operator+=(const Shore &other);
    /// sum operator, this make a weighted of `shore`, using `number` as weight
    Shore operator+(const Shore &other) const;
};


#endif //PIECECLASS_SHORE_H
