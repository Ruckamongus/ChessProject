#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

//an array of possible moves in a given position
typedef moveList {
    int totalMoves;
    move moveArray[]; //possibly later; the moves at the beginning of this array may be prioritised in the search
}

moveList getLegalMoves(const Game g);


#endif // SEARCH_H_INCLUDED
