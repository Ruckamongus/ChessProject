#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

extern "C"
{
    #define MAX_SEARCH_DEPTH 6
    //an array of possible moves in a given position
    struct moveList {
        int totalMoves; //! potential error in the line below. IDK
        move moveArray[]; //possibly later; the moves at the beginning of this array may be prioritised in the search
    };

    //typedef moveList* MoveList;

    moveList getLegalMoves(const Game g);
    move getBestMove(const Game g); //possibly a struct to return a set of best moves
}


#endif // SEARCH_H_INCLUDED
