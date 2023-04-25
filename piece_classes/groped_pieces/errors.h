//
// Created by luca on 4/25/23.
//

#ifndef PIECECLASS_ERRORS_H
#define PIECECLASS_ERRORS_H

#include <iostream>

/// execution return when the avrege shore of all the pieces is to low
class AvregeIsToLow: public std::exception{
};

/*
 * this set of errors is designed to understand witch piece made the creation of the group piece crash.
 * if i had to test all possible 2x2 combinations. the complexity would be O(n^2). but if the top left piece
 * dose not match the top right piece, testing the other 2 pieces is useless, so i skip the calculation.
 * do do that i need to know the exact piece that has failed, do do that i just match the specific error
 * */


/// execution returned when the top right piece dose not match with the top left, so testing all other combinations is useless
class TopRightIsImpossible: public std::exception{};

/// execution returned when the bottom right piece dose not match with the top right, so testing all other combinations is useless
class BottomRightIsImpossible:  public std::exception{};

/// execution returned when the bottom left piece dose not match with the top left, so testing all other combinations is useless
class BottomLeftIsImpossible:  public std::exception{};




#endif //PIECECLASS_ERRORS_H
