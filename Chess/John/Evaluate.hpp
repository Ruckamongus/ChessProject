#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED

extern "C"
{
    #include <Chess\John\JohnDefinitions.hpp>
    double evaluatePosition(const Game g); //positive for white advantage, negative for black advantage
}

#endif // EVALUATE_H_INCLUDED
