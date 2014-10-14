//search functions
#include <Chess\John\JohnDefinitions.hpp>
extern "C"
{
    moveList getLegalMoves(const Game g) {

    }

    move getBestMove(const Game g, int currentDepth) {
        if (currentDepth == MAX_SEARCH_DEPTH) {
            exit;
        }
        /*
        need to consider the data how we're passing it around


        evaluatePosition()

        */
    }
}
