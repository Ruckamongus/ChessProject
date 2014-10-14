#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

extern "C"
{
    #define MAX_SEARCH_DEPTH 2
    #define MAX_MOVES_FROM_ONE_POSITION 100
    //an array of possible moves in a given position
    typedef struct _moveList {
        int totalMoves; //! change the data type to a linked list later
        move moveArray[MAX_MOVES_FROM_ONE_POSITION]; //possibly later; the moves at the beginning of this array may be prioritised in the search
    } moveList;

    typedef moveList* MoveList;

    moveList getLegalMoves(const Game g);
    move getBestMove(const Game g); //possibly a struct to return a set of best moves
    double minimax(const Game g, int depth, bool playerIsWhite);
}


#endif // SEARCH_H_INCLUDED
