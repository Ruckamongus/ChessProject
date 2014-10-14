//search functions
#include <Chess\John\JohnDefinitions.hpp>
#include <Chess\John\Search.hpp>
#include <Chess\John\Evaluate.hpp>

extern "C"
{
    moveList getLegalMoves(const Game g) {
        moveList returnMoveSet;
        returnMoveSet.totalMoves = 0;

        //I'm aware how fucked this looks
        int i, j, x, y;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (g->board[i][j] != EMPTY) {
                    move newMove;
                    newMove.xFrom = i;
                    newMove.yFrom = j;
                    for (x = 0; x < 8; x++) {
                        for (y = 0; y < 8; y++) {
                            newMove.xTo = x;
                            newMove.yTo = y;
                            //printf("Move testing, '%d' '%d' '%d' '%d'\n", newMove.xFrom, newMove.yFrom, newMove.xTo, newMove.yTo);
                            if (isMoveLegal(g, newMove, 1)) {
                                returnMoveSet.moveArray[returnMoveSet.totalMoves] = newMove;
                                returnMoveSet.totalMoves++;
                            }
                        }
                    }
                }
            }
        }

        printf("total possible moves in this function call; %d\n", returnMoveSet.totalMoves);
        return returnMoveSet;
    }

    //move getBestMove(const Game g, int currentDepth) {
    //    if (currentDepth == MAX_SEARCH_DEPTH) {
    //        exit;
    //    }
        /*
        need to consider the data how we're passing it around


        evaluatePosition()


        idea; have a queue data type. we start at a position and
        look at all of the moves from that position. we add them all
        to the queue along with the moves that have been made to get
        to that position
        then we pop the current position off the queue
        now with the new position coming off from the top of the queue
        we are one 'move' deep, it is a breadth first search algorithm
        we evaluate the board and work out how we are going

        */

    // }

    //take the minimax algorithm
    //needs some malloc'ing
    int fuck = 0; //debug
    double minimax(const Game g, int depth, bool playerIsWhite) {
        double bestScore;
        if (depth == 0) {
            bestScore = evaluatePosition(g);
            return bestScore;
        }

        //we can recode this later for optimization with the if (player is White)
        bestScore = (playerIsWhite == TRUE) ? (-9999) : (9999);
        printf("Debug: Starting new moveSet generation\nfuck: %d\n", fuck);
        fuck++;

        moveList moveSet = getLegalMoves(g);
        for (int i = 0; i < moveSet.totalMoves; i++) {
            Game boardCopy = (Game) malloc(sizeof(game)); //obvious problems with memory usage are obvious
            boardCopy = g;
                printf("\n\n"); printGame(boardCopy); printf("\n\n");
            doMove(boardCopy, moveSet.moveArray[i], 0); //doMove doesn't alternate the turn
            //alternate turn
            if (boardCopy->whoseMove == COLOR_WHITE) {
                boardCopy->whoseMove = COLOR_BLACK;
            } else {
                boardCopy->whoseMove = COLOR_WHITE;
            }

            double value = minimax(boardCopy, depth - 1, !playerIsWhite);
            free(boardCopy);

            if (playerIsWhite) {
                bestScore = MAX(bestScore, value);
            } else {
                bestScore = MIN(bestScore, value);
            }
        }
        return bestScore;
    }
}
