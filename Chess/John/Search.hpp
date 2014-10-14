#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

extern "C"
{
    //an array of possible moves in a given position
    struct moveList {
        int totalMoves;
        move moveArray[]; //possibly later; the moves at the beginning of this array may be prioritised in the search
    }

    //typedef moveList* MoveList;

    moveList getLegalMoves(const Game g);
}


#endif // SEARCH_H_INCLUDED
