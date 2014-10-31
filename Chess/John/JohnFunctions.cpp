#include <Chess\John\JohnDefinitions.hpp>
#include <Chess\John\Evaluate.hpp>
#include <Chess\John\Search.hpp>
#include <stdlib.h>

extern "C"
{
    bool intIsOdd(size_t Int)
    {
        return (Int % 2);
    }

    pieceValue charToPiece960(char Char, bool White)
    {
        switch (Char)
        {
            case 'K': if (White) return W_KING; return B_KING;
            case 'Q': if (White) return W_QUEEN; return B_QUEEN;
            case 'R': if (White) return W_ROOK; return B_ROOK;
            case 'N': if (White) return W_KNIGHT; return B_KNIGHT;
            case 'B': if (White) return W_BISHOP; return B_BISHOP;
            default: return EMPTY;
        }
    }


    void randomize960(Game g)
    {
        //We're assuming the newGame() function has already been called.
        //The bishops must be placed on opposite-color squares.
        //The king must be placed on a square between the rooks.
        char Pieces[] = {'B', 'B', 'R', 'R', 'N', 'N', 'Q', 'K'};
        bool rightRook = 0, leftRook = 0, King = 0, whiteBishop = 0, blackBishop = 0;

        for (size_t i = 0; i < 8; ++i)
        {
            g->board[i][0] = '0';
            char curr = '0';
            size_t Index;

            while (curr == '0')
            {
                Index = rand() % 8;
                curr = Pieces[Index];
            }

            if (curr == 'K')
            {
                if (i > 0 && i < 7)
                {
                    if (leftRook && !rightRook)
                    {
                        King = 1;
                        g->board[i][0] = 'K';
                        Pieces[Index] = '0';
                    }
                }
            }

            if (curr == 'R')
            {
                if (!leftRook)
                {
                    leftRook = 1;
                    g->board[i][0] = 'R';
                    Pieces[Index] = '0';
                }

                if (!rightRook && King)//King must be placed between the rooks
                {
                    rightRook = 1;
                    g->board[i][0] = 'R';
                    Pieces[Index] = '0';
                }
            }

            if (curr == 'B')
            {
                if (intIsOdd(i) && !blackBishop)
                {
                    blackBishop = i;
                    g->board[i][0] = 'B';
                    Pieces[Index] = '0';
                }

                if (!intIsOdd(i) && !whiteBishop)
                {
                    whiteBishop = i;
                    g->board[i][0] = 'B';
                    Pieces[Index] = '0';
                }
            }

            if (curr == 'Q' || curr == 'N')
            {
                g->board[i][0] = curr;
                Pieces[Index] = '0';
            }
        }

        if (King && blackBishop && whiteBishop)
        {
            bool FuckedUp = 0;
            for (size_t i = 0; i < 8; ++i)
            {
                char Switch = g->board[i][0];
                g->board[i][0] = charToPiece960(Switch, 1);
                g->board[i][7] = charToPiece960(Switch, 0);

                if (g->board[i][0] == EMPTY) FuckedUp = 1;
            }

            if (FuckedUp) randomize960(g);
        }
        else
        randomize960(g);
    }

    Game newGame()
    {
        Game g = (Game) malloc(sizeof(game));
        int i, j;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                //printf("wut2\n");
                if (j == 1) {
                    g->board[i][1] = W_PAWN;
                } else if (j == 6) {
                    g->board[i][6] = B_PAWN;
                } else {
                    g->board[i][j] = EMPTY;
                }
            }
        }
        g->board[0][0] = W_ROOK;
        g->board[7][0] = W_ROOK;
        g->board[1][0] = W_KNIGHT;
        g->board[6][0] = W_KNIGHT;
        g->board[2][0] = W_BISHOP;
        g->board[5][0] = W_BISHOP;
        g->board[3][0] = W_QUEEN;
        g->board[4][0] = W_KING;

        g->board[0][7] = B_ROOK;
        g->board[7][7] = B_ROOK;
        g->board[1][7] = B_KNIGHT;
        g->board[6][7] = B_KNIGHT;
        g->board[2][7] = B_BISHOP;
        g->board[5][7] = B_BISHOP;
        g->board[3][7] = B_QUEEN;
        g->board[4][7] = B_KING;

        g->whoseMove = COLOR_WHITE;
        g->canEnPassant = NO_ENPASSANT;
        g->wKingMoved = FALSE;
        g->bKingMoved = FALSE;
        g->bLeftRookMoved = FALSE;
        g->bRightRookMoved = FALSE;
        g->wLeftRookMoved = FALSE;
        g->wRightRookMoved = FALSE;
        return g;
    }

    void deleteGame(Game g)
    {
        free(g);
    }

    move getInput() {
        printf("Enter your move. Format: xyXY where xy are the starting coords, XY destination coords\n");
        move myMove;
        scanf("\n%c%c%c%c", &myMove.xFrom, &myMove.yFrom, &myMove.xTo, &myMove.yTo); //newline at start from last output
        if (!(myMove.xFrom >= 48 && myMove.xFrom <= 55) || !(myMove.xTo >= 48 && myMove.xTo <= 55)
            || !(myMove.yFrom >= 48 && myMove.yFrom <= 55) || !(myMove.yTo >= 48 && myMove.yTo <= 55)) {
            printf("Invalid input: '%c %c %c %c'\n", myMove.xFrom, myMove.yFrom, myMove.xTo, myMove.yTo);
            assert(0);
        }
        myMove.xFrom -= 48; //ASCII
        myMove.yFrom -= 48;
        myMove.xTo -= 48;
        myMove.yTo -= 48;

        return myMove;
    }

    //returns 0 if a move wasn't made, 1 for standard move, 2 for kingside castle, 3 for queenside castle
    int makeMove(Game g, move m, bool noVerbose)
    {
        int returnVal;
        if (isMoveLegal(g, m))
        {
            if (!noVerbose) printf("Move was legal.\n");

            returnVal = doMove(g, m);
            //alternate turn
            if (g->whoseMove == COLOR_WHITE) {
                g->whoseMove = COLOR_BLACK;
            } else {
                g->whoseMove = COLOR_WHITE;
            }

            moveList moves = getLegalMoves(g);
            if (moves.totalMoves == 0) {
                printf("We are in checkmate. \n");
                returnVal = MOVE_CHECKMATE;
            }
            if (!noVerbose) {
                printf("Total moves: %d\n", moves.totalMoves);
            }
            /*
            double score = evaluatePosition(g);
            printf("Current board score (static): %f\n", score);
            bool isWhite;
            if (g->whoseMove == COLOR_BLACK) {isWhite = FALSE;}
            else if (g->whoseMove == COLOR_WHITE) {isWhite = TRUE;}
            else {printf("The fuck.\n"); assert(0);}
            score = minimax(g, MAX_SEARCH_DEPTH, isWhite);
            printf("Current board score according to the minimax search: %f\n", score);
            */

            return returnVal;
        } else {
            if (!noVerbose) printf("An illegal move attempt has been made.\n");
            returnVal = MOVE_NONE_MADE;
        }
        return returnVal;
    }

    int doMove(Game g, move m, bool noVerbose) //does not alternate move turn
    {
        int returnVal = 0;
        pieceValue piece = g->board[m.xFrom][m.yFrom];
        g->board[m.xTo][m.yTo] = piece;
        g->board[m.xFrom][m.yFrom] = EMPTY;
        if (m.xFrom == 0) {
            if (m.yFrom == 0) {
                if (piece == W_ROOK) {
                    g->wLeftRookMoved = TRUE;
                }
            } else if (m.yFrom == 7) {
                if (piece == B_ROOK) {
                    g->bLeftRookMoved = TRUE;
                }
            }
        } else if (m.xFrom == 7) {
            if (m.yFrom == 0) {
                if (piece == W_ROOK) {
                    g->wRightRookMoved = TRUE;
                }
            } else if (m.yFrom == 7) {
                if (piece == B_ROOK) {
                    g->bRightRookMoved = TRUE;
                }
            }
        }
        if (piece == B_KING) {
            g->bKingMoved = TRUE;
            if (m.xTo - m.xFrom == 2) { //kingside castle
                g->board[7][7] = EMPTY;
                g->board[5][7] = B_ROOK;
                returnVal = MOVE_IS_KCASTLE;
            } else if (m.xTo - m.xFrom == -2) { //queenside castling
                g->board[0][7] = EMPTY;
                g->board[3][7] = B_ROOK;
                returnVal = MOVE_IS_QCASTLE;
            }
        } else if (piece == W_KING) {
            g->wKingMoved = TRUE;
            if (m.xTo - m.xFrom == 2) { //kingside castle
                g->board[7][0] = EMPTY;
                g->board[5][0] = W_ROOK;
                returnVal = MOVE_IS_KCASTLE;
            } else if (m.xTo - m.xFrom == -2) { //queenside castling
                g->board[0][0] = EMPTY;
                g->board[3][0] = W_ROOK;
                returnVal = MOVE_IS_QCASTLE;
            }
        } else if (piece == B_PAWN) { //set up en passant
            if (m.yFrom == 6 && m.yTo == 4) {
                g->canEnPassant = m.xTo;
            } else {
                if (m.yFrom == 3 && ((m.xTo == m.xFrom - 1) || (m.xTo == m.xFrom + 1))) { //en passant
                    g->board[m.xTo][m.yFrom] = EMPTY;
                    if (!noVerbose) printf("En passant.\n");
                }
                g->canEnPassant = NO_ENPASSANT;

            }
        } else if (piece == W_PAWN) {
            if (m.yFrom == 1 && m.yTo == 3) {
                g->canEnPassant = m.xTo;
            } else {
                if (m.yFrom == 4 && ((m.xTo == m.xFrom - 1) || (m.xTo == m.xFrom + 1))) { //en passant
                    g->board[m.xTo][m.yFrom] = EMPTY;
                    if (!noVerbose) printf("En passant.\n");
                }
                g->canEnPassant = NO_ENPASSANT;
            }
        } else {
            g->canEnPassant = NO_ENPASSANT;
        }

        return returnVal;
    }

    emptyBlackWhite getSquare(pieceValue piece)
    {
        switch (piece)
        {
            case EMPTY:     return COLOR_NONE; //empty
            case B_PAWN:    return COLOR_BLACK;
            case B_KNIGHT:  return COLOR_BLACK;
            case B_BISHOP:  return COLOR_BLACK;
            case B_ROOK:    return COLOR_BLACK;
            case B_QUEEN:   return COLOR_BLACK;
            case B_KING:    return COLOR_BLACK;
            case W_PAWN:    return COLOR_WHITE;
            case W_KNIGHT:  return COLOR_WHITE;
            case W_BISHOP:  return COLOR_WHITE;
            case W_ROOK:    return COLOR_WHITE;
            case W_QUEEN:   return COLOR_WHITE;
            case W_KING:    return COLOR_WHITE;
            default: printf("Wrong input. piece: %d\n", piece); assert(0);
        }
        return 0;
    }

    int isInCheck(const Game g, emptyBlackWhite colorChecking) {
        if (colorChecking == COLOR_BLACK) {
            boardCoord i, j, kingX = 100, kingY = 100; //init to stop compiler grumbling
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    if (g->board[i][j] == B_KING) {
                        kingX = i;
                        kingY = j;
                        break; //will this break both loops?
                    }
                }
            }
            assert(kingX != 100 && kingY != 100); //there should always be a king on the board
            //check by pawns
            if (kingY != 0) { //we are never going to be in check by pawns if kingY == 0
                if (kingX > 0) {
                    if (g->board[kingX - 1][kingY - 1] == W_PAWN) {
                        return TRUE;
                    }
                }
                if (kingX < 7) {
                    if (g->board[kingX + 1][kingY - 1] == W_PAWN) {
                        return TRUE;
                    }
                }
            }
            //check by moving into other kings range
            {
                int _left, _right, _up, _down;
                _left = (kingX != 0);
                _right = (kingX != 7);
                _down = (kingY != 0);
                _up = (kingY != 7);
                if (_left) {
                    if (g->board[kingX - 1][kingY] == W_KING) {
                        return TRUE;
                    }
                    if (_up) {
                        if (g->board[kingX - 1][kingY + 1] == W_KING) {
                            return TRUE;
                        }
                    }
                    if (_down) {
                        if (g->board[kingX - 1][kingY - 1] == W_KING) {
                            return TRUE;
                        }
                    }
                }
                if (_right) {
                    if (g->board[kingX + 1][kingY] == W_KING) {
                        return TRUE;
                    }
                    if (_up) {
                        if (g->board[kingX + 1][kingY + 1] == W_KING) {
                            return TRUE;
                        }
                    }
                    if (_down) {
                        if (g->board[kingX + 1][kingY - 1] == W_KING) {
                            return TRUE;
                        }
                    }
                }
                if (_down) {
                    if (g->board[kingX][kingY - 1] == W_KING) {
                        return TRUE;
                    }
                }
                if (_up) {
                    if (g->board[kingX][kingY + 1] == W_KING) {
                        return TRUE;
                    }
                }
            }
            //check by rook/queen straight movement
            {
                int _X, _Y;
                _X = kingX - 1;
                while (_X >= 0) { //rook/queen to left of me
                    if (g->board[_X][kingY] != EMPTY) {
                        if (g->board[_X][kingY] == W_ROOK || g->board[_X][kingY] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X--;
                }
                _X = kingX + 1;
                while (_X < 8) { //rook/queen to right of me
                    if (g->board[_X][kingY] != EMPTY) {
                        if (g->board[_X][kingY] == W_ROOK || g->board[_X][kingY] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X++;
                }
                _Y = kingY - 1;
                while (_Y >= 0) { //rook/queen to left of me
                    if (g->board[kingX][_Y] != EMPTY) {
                        if (g->board[kingX][_Y] == W_ROOK || g->board[kingX][_Y] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _Y--;
                }
                _Y = kingY + 1;
                while (_Y < 8) { //rook/queen to right of me
                    if (g->board[kingX][_Y] != EMPTY) {
                        if (g->board[kingX][_Y] == W_ROOK || g->board[kingX][_Y] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _Y++;
                }
            }
            //check by bishop/queen diagonal
            {
                int _X, _Y;
                _X = kingX - 1;
                _Y = kingY - 1;
                while (_X >= 0 && _Y >= 0) { //bishop/queen lower and left of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == W_BISHOP || g->board[_X][_Y] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X--;
                    _Y--;
                }
                _X = kingX + 1;
                _Y = kingY - 1;
                while (_X < 8 && _Y >= 0) { //bishop/queen lower and right of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == W_BISHOP || g->board[_X][_Y] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X++;
                    _Y--;
                }
                _X = kingX - 1;
                _Y = kingY + 1;
                while (_X >= 0 && _Y < 8) { //bishop/queen higher and left of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == W_BISHOP || g->board[_X][_Y] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X--;
                    _Y++;
                }
                _X = kingX + 1;
                _Y = kingY + 1;
                while (_X < 8 && _Y < 8) { //bishop/queen higher and right of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == W_BISHOP || g->board[_X][_Y] == W_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X++;
                    _Y++;
                }
            }
            //check by knight, keep in these braces to keep these variables temporary...
            {
                int tempXp2, tempXp1, tempXm1, tempXm2, tempYp2, tempYp1, tempYm1, tempYm2;
                tempXp2 = (kingX + 2 < 8);
                tempXp1 = (kingX + 1 < 8);
                tempXm1 = (kingX - 1 >= 0);
                tempXm2 = (kingX - 2 >= 0);
                tempYp2 = (kingY + 2 < 8);
                tempYp1 = (kingY + 1 < 8);
                tempYm1 = (kingY - 1 >= 0);
                tempYm2 = (kingY - 2 >= 0);

                //looks ugly as hell, but works
                if (tempXp2) {
                    if (tempYp1) {
                        if (g->board[kingX + 2][kingY + 1] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm1) {
                        if (g->board[kingX + 2][kingY - 1] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
                if (tempXp1) {
                    if (tempYp2) {
                        if (g->board[kingX + 1][kingY + 2] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm2) {
                        if (g->board[kingX + 1][kingY - 2] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
                if (tempXm1) {
                    if (tempYp2) {
                        if (g->board[kingX - 1][kingY + 2] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm2) {
                        if (g->board[kingX - 1][kingY - 2] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
                if (tempXm2) {
                    if (tempYp1) {
                        if (g->board[kingX - 2][kingY + 1] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm1) {
                        if (g->board[kingX - 2][kingY - 1] == W_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
            }
        } else { //white to check for
            boardCoord i, j, kingX = 100, kingY = 100; //init to stop compiler grumbling
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    if (g->board[i][j] == W_KING) {
                        kingX = i;
                        kingY = j;
                        break; //will this break both loops?
                    }
                }
            }
            assert(kingX != 100 && kingY != 100); //there should always be a king on the board
            //check by pawns
            if (kingY != 7) { //we are never going to be in check by pawns if kingY == 0
                if (kingX > 0) {
                    if (g->board[kingX - 1][kingY + 1] == B_PAWN) {
                        return TRUE;
                    }
                }
                if (kingX < 7) {
                    if (g->board[kingX + 1][kingY + 1] == B_PAWN) {
                        return TRUE;
                    }
                }
            }
            //check by moving into other kings range
            {
                int _left, _right, _up, _down;
                _left = (kingX != 0);
                _right = (kingX != 7);
                _down = (kingY != 0);
                _up = (kingY != 7);
                if (_left) {
                    if (g->board[kingX - 1][kingY] == B_KING) {
                        return TRUE;
                    }
                    if (_up) {
                        if (g->board[kingX - 1][kingY + 1] == B_KING) {
                            return TRUE;
                        }
                    }
                    if (_down) {
                        if (g->board[kingX - 1][kingY - 1] == B_KING) {
                            return TRUE;
                        }
                    }
                }
                if (_right) {
                    if (g->board[kingX + 1][kingY] == B_KING) {
                        return TRUE;
                    }
                    if (_up) {
                        if (g->board[kingX + 1][kingY + 1] == B_KING) {
                            return TRUE;
                        }
                    }
                    if (_down) {
                        if (g->board[kingX + 1][kingY - 1] == B_KING) {
                            return TRUE;
                        }
                    }
                }
                if (_down) {
                    if (g->board[kingX][kingY - 1] == B_KING) {
                        return TRUE;
                    }
                }
                if (_up) {
                    if (g->board[kingX][kingY + 1] == B_KING) {
                        return TRUE;
                    }
                }
            }
            //check by rook/queen straight movement
            {
                int _X, _Y;
                _X = kingX - 1;
                while (_X >= 0) { //rook/queen to left of me
                    if (g->board[_X][kingY] != EMPTY) {
                        if (g->board[_X][kingY] == B_ROOK || g->board[_X][kingY] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X--;
                }
                _X = kingX + 1;
                while (_X < 8) { //rook/queen to right of me
                    if (g->board[_X][kingY] != EMPTY) {
                        if (g->board[_X][kingY] == B_ROOK || g->board[_X][kingY] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X++;
                }
                _Y = kingY - 1;
                while (_Y >= 0) { //rook/queen to left of me
                    if (g->board[kingX][_Y] != EMPTY) {
                        if (g->board[kingX][_Y] == B_ROOK || g->board[kingX][_Y] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _Y--;
                }
                _Y = kingY + 1;
                while (_Y < 8) { //rook/queen to right of me
                    if (g->board[kingX][_Y] != EMPTY) {
                        if (g->board[kingX][_Y] == B_ROOK || g->board[kingX][_Y] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _Y++;
                }
            }
            //check by bishop/queen diagonal
            {
                int _X, _Y;
                _X = kingX - 1;
                _Y = kingY - 1;
                while (_X >= 0 && _Y >= 0) { //bishop/queen lower and left of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == B_BISHOP || g->board[_X][_Y] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X--;
                    _Y--;
                }
                _X = kingX + 1;
                _Y = kingY - 1;
                while (_X < 8 && _Y >= 0) { //bishop/queen lower and right of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == B_BISHOP || g->board[_X][_Y] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X++;
                    _Y--;
                }
                _X = kingX - 1;
                _Y = kingY + 1;
                while (_X >= 0 && _Y < 8) { //bishop/queen higher and left of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == B_BISHOP || g->board[_X][_Y] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X--;
                    _Y++;
                }
                _X = kingX + 1;
                _Y = kingY + 1;
                while (_X < 8 && _Y < 8) { //bishop/queen higher and right of me
                    if (g->board[_X][_Y] != EMPTY) {
                        if (g->board[_X][_Y] == B_BISHOP || g->board[_X][_Y] == B_QUEEN) {
                            return TRUE;
                        } else { //we have an obstacle piece
                            break;
                        }
                    }
                    _X++;
                    _Y++;
                }
            }
            //check by knight, keep in these braces to keep these variables temporary...
            {
                int tempXp2, tempXp1, tempXm1, tempXm2, tempYp2, tempYp1, tempYm1, tempYm2;
                tempXp2 = (kingX + 2 < 8);
                tempXp1 = (kingX + 1 < 8);
                tempXm1 = (kingX - 1 >= 0);
                tempXm2 = (kingX - 2 >= 0);
                tempYp2 = (kingY + 2 < 8);
                tempYp1 = (kingY + 1 < 8);
                tempYm1 = (kingY - 1 >= 0);
                tempYm2 = (kingY - 2 >= 0);

                //looks ugly as hell, but works
                if (tempXp2) {
                    if (tempYp1) {
                        if (g->board[kingX + 2][kingY + 1] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm1) {
                        if (g->board[kingX + 2][kingY - 1] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
                if (tempXp1) {
                    if (tempYp2) {
                        if (g->board[kingX + 1][kingY + 2] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm2) {
                        if (g->board[kingX + 1][kingY - 2] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
                if (tempXm1) {
                    if (tempYp2) {
                        if (g->board[kingX - 1][kingY + 2] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm2) {
                        if (g->board[kingX - 1][kingY - 2] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
                if (tempXm2) {
                    if (tempYp1) {
                        if (g->board[kingX - 2][kingY + 1] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                    if (tempYm1) {
                        if (g->board[kingX - 2][kingY - 1] == B_KNIGHT) {
                            return TRUE;
                        }
                    }
                }
            }
        }
        return FALSE;
    }

    //unfinished
    int isMoveLegal(const Game g, move m, bool noVerbose)
    {
        //printf("\n%d %d %d %d\n\n", m.xFrom, m.yFrom, m.xTo, m.yTo);
        //need to test for all that it doesn't put the king in check
        if (m.xFrom < 0 || m.xTo < 0 || m.yFrom < 0 || m.yTo < 0
            || m.xFrom > 7 || m.xTo > 7 || m.yFrom > 7 || m.yTo > 7) {
            return FALSE;
        }

        pieceValue piece = g->board[m.xFrom][m.yFrom];
        //are we trying to move a piece? Is it that correct color?
        emptyBlackWhite myPieceColor = getSquare(piece);
        if (myPieceColor != g->whoseMove) {
            if (!noVerbose) printf("Wrong color's turn.\n");
            return FALSE;
        }

        //cant take own pieces
        if (getSquare(g->board[m.xTo][m.yTo]) == g->whoseMove) {
            return FALSE;
        }

        //make the move (even if illegal), and see if it leaves you in check. If the move
        //was illegal, then it will be caught later anyway

        if (piece == B_KING || piece == W_KING) { //test can't castle through checks. Doesn't immediately rigorously test stuff if if we've moved our king etc yet
            if (m.xTo - m.xFrom == 2) {
                if (isInCheck(g, g->whoseMove) == TRUE) { //can't castle out of check
                    return FALSE;
                }
                //path of check
                move testCheckMove;
                testCheckMove.xFrom = m.xFrom;
                testCheckMove.yFrom = m.yFrom;
                testCheckMove.xTo = m.xFrom + 1;
                testCheckMove.yTo = m.yFrom;
                //!be super careful of recursion. Potential cause of bugs big time
                if (isMoveLegal(g, testCheckMove, TRUE) == FALSE) {
                    printf("Castling is false because we are moving through a check.\n");
                    return FALSE;
                }
            } else if (m.xTo - m.xFrom == -2) {
                if (isInCheck(g, g->whoseMove) == TRUE) { //can't castle out of check
                    return FALSE;
                }
                move testCheckMove;
                testCheckMove.xFrom = m.xFrom;
                testCheckMove.yFrom = m.yFrom;
                testCheckMove.xTo = m.xFrom - 1;
                testCheckMove.yTo = m.yFrom;
                //!be super careful of recursion. Potential cause of bugs big time
                if (isMoveLegal(g, testCheckMove, TRUE) == FALSE) {
                    printf("Castling is false because we are moving through a check.\n");
                    return FALSE;
                }
            }
        }


        emptyBlackWhite moveIsEnPassant = EMPTY;
        boardCoord priorEnPassant = g->canEnPassant;
        bool priorWKingMoved = g->wKingMoved;
        bool priorBKingMoved = g->bKingMoved;
        bool priorWLeftRookMoved = g->wLeftRookMoved;
        bool priorWRightRookMoved = g->wRightRookMoved;
        bool priorBLeftRookMoved = g->bLeftRookMoved;
        bool priorBRightRookMoved = g->bRightRookMoved;
        if (piece == B_PAWN) {
            if (m.yTo == (m.yFrom - 1) && ((m.xTo == m.xFrom - 1) || (m.xTo == m.xFrom + 1))) {
                if (m.yFrom == 3) {
                    moveIsEnPassant = COLOR_BLACK;
                    if (getSquare(g->board[m.xTo][m.yTo]) == COLOR_WHITE) { //taking a piece
                        return TRUE;
                    }
                    if (m.xTo != priorEnPassant) {
                        if (!noVerbose) printf("Can't en passant here. canENPassanet = %d\n", g->canEnPassant);
                        return FALSE; //this is checked later in the switch, but never will occur, we are quick terminating here. Delete the switch bit later
                    }
                }
            }
        }
        if (piece == W_PAWN) {
            if (m.yTo == (m.yFrom + 1) && ((m.xTo == m.xFrom - 1) || (m.xTo == m.xFrom + 1))) {
                if (m.yFrom == 4) {
                    moveIsEnPassant = COLOR_WHITE;
                    if (getSquare(g->board[m.xTo][m.yTo]) == COLOR_BLACK) { //taking a piece
                        return TRUE;
                    }
                    if (m.xTo != priorEnPassant) {
                        if (!noVerbose) printf("Can't en passant here. canENPassanet = %d\n", g->canEnPassant);
                        return FALSE; //this is checked later in the switch, but never will occur, we are quick terminating here. Delete the switch bit later
                    }
                }
            }
        }
        pieceValue destination = g->board[m.xTo][m.yTo];
        pieceValue pieceAtD1 = g->board[3][0];
        pieceValue pieceAtD8 = g->board[3][7];
        pieceValue pieceAtF1 = g->board[5][0];
        pieceValue pieceAtF8 = g->board[5][7];
        doMove(g, m); //does not change whose turn it is
        int inCheck = isInCheck(g, g->whoseMove);
        //undo move
        //NOTE WE NEED TO WORK HARDER TO UNDO EN PASSANT, CASTLING, PAWN PROMOTION, AND THINK OF OTHERS
        g->board[m.xTo][m.yTo] = destination;
        g->board[m.xFrom][m.yFrom] = piece;
        g->canEnPassant = priorEnPassant;
        g->wKingMoved = priorWKingMoved;
        g->bKingMoved = priorBKingMoved;
        g->wLeftRookMoved = priorWLeftRookMoved;
        g->wRightRookMoved = priorWRightRookMoved;
        g->bLeftRookMoved = priorBLeftRookMoved;
        g->bRightRookMoved = priorBRightRookMoved;
        if (moveIsEnPassant == COLOR_BLACK) {
            g->board[m.xTo][m.yFrom] = W_PAWN;
            if (!inCheck) {
                return TRUE; //we've already done all the calculations for this move
            }
        } else if (moveIsEnPassant == COLOR_WHITE) {
            g->board[m.xTo][m.yFrom] = B_PAWN;
            if (!inCheck) {
                return TRUE; //we've already done all the calculations for this move
            }
        }
        if (piece == B_KING) {
            if (m.xTo - m.xFrom == 2) { //kingside castling
                g->board[7][7] = B_ROOK;
                g->board[5][7] = pieceAtF8;
            } else if (m.xTo - m.xFrom == -2) {//queenside castling
                g->board[0][7] = B_ROOK;
                g->board[3][7] = pieceAtD8;
            }
        } else if (piece == W_KING) {
            if (m.xTo - m.xFrom == 2) { //kingside castling
                g->board[7][0] = W_ROOK;
                g->board[5][0] = pieceAtF1;
            } else if (m.xTo - m.xFrom == -2) {//queenside castling
                g->board[0][0] = W_ROOK;
                g->board[3][0] = pieceAtD1;
            }
        }
        if (inCheck) {
            if (!noVerbose) printf("Wrong by in check\n");
            return FALSE;
        }

        switch (piece) {
            case B_PAWN: {
                //first move
                if ((m.yFrom == 6 && m.yTo == 4) && (m.xTo == m.xFrom)) {
                    if (g->board[m.xTo][5] == EMPTY && g->board[m.xTo][4] == EMPTY) {
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                } else if (m.yTo == (m.yFrom - 1)) {
                    if ((m.xTo == m.xFrom - 1) || (m.xTo == m.xFrom + 1)) {
                        //capturing
                        if (getSquare(g->board[m.xTo][m.yTo]) == COLOR_WHITE) {
                            return TRUE;
                        }
                        //en passant
                        if (m.yFrom == 3 && m.xTo == g->canEnPassant) {
                            if (!noVerbose) printf("this is redundant and should never occur.\n");
                            return TRUE;
                        } else {
                            return FALSE;
                        }
                    } else if (m.xTo == m.xFrom) {
                        //normal one move forward
                        if (g->board[m.xFrom][m.yTo] == EMPTY) {
                            return TRUE;
                        } else {
                            return FALSE;
                        }
                    } else {
                        return FALSE;
                    }
                } else {
                    return FALSE;
                }
                if (!noVerbose) printf("Shouldn't happen.\n"); assert(0);
            }
            case W_PAWN: {
                //first move
                if ((m.yFrom == 1 && m.yTo == 3) && (m.xTo == m.xFrom)) {
                    if (g->board[m.xTo][2] == EMPTY && g->board[m.xTo][3] == EMPTY) {
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                } else if (m.yTo == (m.yFrom + 1)) {
                    if ((m.xTo == m.xFrom - 1) || (m.xTo == m.xFrom + 1)) {
                        //capturing
                        if (getSquare(g->board[m.xTo][m.yTo]) == COLOR_BLACK) {
                            return TRUE;
                        }
                        //en passant
                        if (m.yFrom == 4 && m.xTo == g->canEnPassant) {
                            if (!noVerbose) printf("this is redundant and should never occur.\n");
                            return TRUE;
                        } else {
                            return FALSE;
                        }
                    } else if (m.xTo == m.xFrom) {
                        //normal one move forward
                        if (g->board[m.xFrom][m.yTo] == EMPTY) {
                            return TRUE;
                        } else {
                            return FALSE;
                        }
                    } else {
                        return FALSE;
                    }
                } else {
                    return FALSE;
                }
                if (!noVerbose) printf("Shouldn't happen.\n"); assert(0);
            }
            case B_KNIGHT: case W_KNIGHT: {
                //printf("Knight movement\n");
                char xDisplacement = (char) (m.xTo - m.xFrom);
                char yDisplacement = (char) (m.yTo - m.yFrom);
                if (xDisplacement == 1) {
                    if (yDisplacement == 2 || yDisplacement == -2) {
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                } else if (xDisplacement == -1) {
                    if (yDisplacement == 2 || yDisplacement == -2) {
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                } else if (xDisplacement == 2) {
                    if (yDisplacement == 1 || yDisplacement == -1) {
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                } else if (xDisplacement == -2) {
                    if (yDisplacement == 1 || yDisplacement == -1) {
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                } else {
                    if (!noVerbose) printf("Knight movement: falsies\n");
                    return FALSE;
                }
                if (!noVerbose) printf("Shouldn't happen.\n"); assert(0);
            }
            case B_BISHOP: case W_BISHOP: {
                //printf("Bishop movement\n");
                char xDisplacement = (char) (m.xTo - m.xFrom);
                char yDisplacement = (char) (m.yTo - m.yFrom);
                if (xDisplacement != yDisplacement && xDisplacement != -yDisplacement) {
                    return FALSE;
                } else if (xDisplacement == 0) { //catch null moves
                    return FALSE;
                } else {
                    //check for obstacles
                    int horizontalSign = ((xDisplacement > 0) * 2) - 1; //gets the sign
                    int verticalSign = ((yDisplacement > 0) * 2) - 1;
                    int absDistance = horizontalSign*xDisplacement; //gets the magnitude of the diagonal
                    int i = 1;
                    while (i < absDistance) { //or yDisplacement. Check out the path to destination, but not the actual destination
                        if (g->board[m.xFrom + i*horizontalSign][m.yFrom + i*verticalSign] != EMPTY) {
                            //printf("Error is: (%d,%d).\n", m.xFrom + i*horizontalSign, m.yFrom + i*verticalSign);
                            return FALSE;
                        }
                        i++;
                    }
                    return TRUE;
                }
                if (!noVerbose) printf("Shouldn't happen.\n"); assert(0);
            }
            case B_ROOK: case W_ROOK: {
                char xDisplacement = (char) (m.xTo - m.xFrom);
                char yDisplacement = (char) (m.yTo - m.yFrom);
                if ((xDisplacement == 0 && yDisplacement == 0) || (xDisplacement != 0 && yDisplacement != 0)) { //only move on one axis, ensure non-stationary move
                    return FALSE;
                } else {
                    int i, j;
                    //scroll through the files or ranks to detect if we're passing through pieces
                    if (xDisplacement > 0) {
                        for (i = m.xFrom + 1; i < m.xTo; i++) { //m.xFrom + 1 as we do not want to include ourselves
                            if (g->board[i][m.yFrom] != EMPTY) {
                                return FALSE;
                            }
                        }
                    } else if (xDisplacement < 0){
                        for (i = m.xFrom - 1; i > m.xTo; i--) {
                            if (g->board[i][m.yFrom] != EMPTY) {
                                return FALSE;
                            }
                        }
                    } else if (yDisplacement > 0) {
                        for (j = m.yFrom + 1; j < m.yTo; j++) {
                            if (g->board[m.xFrom][j] != EMPTY) {
                                return FALSE;
                            }
                        }
                    } else {
                        for (j = m.yFrom - 1; j > m.yTo; j--) {
                            if (g->board[m.xFrom][j] != EMPTY) {
                                return FALSE;
                            }
                        }
                    }
                    return TRUE;
                }
            }
            case B_QUEEN: case W_QUEEN: {
                char xDisplacement = (char) (m.xTo - m.xFrom);
                char yDisplacement = (char) (m.yTo - m.yFrom);
                if (xDisplacement != yDisplacement && xDisplacement != -yDisplacement) { //horizontal movement
                    if (xDisplacement != 0 && yDisplacement != 0) { //only move on one axis
                        return FALSE;
                    } else {
                        int i, j;
                        //scroll through the files or ranks to detect if we're passing through pieces
                        if (xDisplacement > 0) {
                            for (i = m.xFrom + 1; i < m.xTo; i++) { //m.xFrom + 1 as we do not want to include ourselves
                                if (g->board[i][m.yFrom] != EMPTY) {
                                    return FALSE;
                                }
                            }
                        } else if (xDisplacement < 0){
                            for (i = m.xFrom - 1; i > m.xTo; i--) {
                                if (g->board[i][m.yFrom] != EMPTY) {
                                    return FALSE;
                                }
                            }
                        } else if (yDisplacement > 0) {
                            for (j = m.yFrom + 1; j < m.yTo; j++) {
                                if (g->board[m.xFrom][j] != EMPTY) {
                                    return FALSE;
                                }
                            }
                        } else {
                            for (j = m.yFrom - 1; j > m.yTo; j--) {
                                if (g->board[m.xFrom][j] != EMPTY) {
                                    return FALSE;
                                }
                            }
                        }
                        return TRUE;
                    }
                } else if (xDisplacement == 0) { //catch null moves
                    return FALSE;
                } else { //diagonal movement
                    //check for obstacles
                    int horizontalSign = ((xDisplacement > 0) * 2) - 1; //gets the sign
                    int verticalSign = ((yDisplacement > 0) * 2) - 1;
                    int absDistance = horizontalSign*xDisplacement; //gets the magnitude of the diagonal
                    int i = 1;
                    while (i < absDistance) { //or yDisplacement. Check out the path to destination, but not the actual destination
                        if (g->board[m.xFrom + i*horizontalSign][m.yFrom + i*verticalSign] != EMPTY) {
                            //printf("Error is: (%d,%d).\n", m.xFrom + i*horizontalSign, m.yFrom + i*verticalSign);
                            return FALSE;
                        }
                        i++;
                    }
                    return TRUE;
                }
                if (!noVerbose) printf("Shouldn't happen.\n"); assert(0);
            }
            case B_KING: {
                char xDisplacement = (char) (m.xTo - m.xFrom);
                char yDisplacement = (char) (m.yTo - m.yFrom);
                int horizontalSign = ((xDisplacement > 0) * 2) - 1; //gets the sign
                int verticalSign = ((yDisplacement > 0) * 2) - 1;
                int absXDistance = horizontalSign*xDisplacement; //magnitude
                int absYDistance = verticalSign*yDisplacement;
                if (absXDistance == 2) { //castling, potentially split it up
                    if (g->bKingMoved == TRUE) { //we can't castle if the king has already moved
                        return FALSE;
                    }
                    if (absYDistance != 0) {
                        return FALSE;
                    }
                    if (horizontalSign == 1) {//kingside castle
                        if (g->bRightRookMoved == TRUE || g->board[7][7] != B_ROOK) {
                            printf("Debug: The rook has already been moved or taken.\n");
                            return FALSE;
                        }
                        //piece obstruction of the castling linear path
                        if (g->board[m.xFrom + 1][m.yFrom] != EMPTY || g->board[m.xFrom + 2][m.yFrom] != EMPTY ) { //not hardcoded for chess960 possibly in future. Would need amendments
                            return FALSE;
                        }
                        //we need to check if the king is moving through check and disallow this. Done elsewhere
                        return TRUE;
                    } else { //queenside castling
                        if (g->bLeftRookMoved == TRUE || g->board[0][7] != B_ROOK) {
                            printf("Debug: The rook has already been moved or taken.\n");
                            return FALSE;
                        }
                        if (g->board[m.xFrom - 1][m.yFrom] != EMPTY || g->board[m.xFrom - 2][m.yFrom] != EMPTY || g->board[m.xFrom - 3][m.yFrom] != EMPTY ) {
                            return FALSE;
                        }
                        //we need to check if the king is moving through check and disallow this. Done elsewhere
                        return TRUE;
                    }
                } else if (absXDistance > 1 || absYDistance > 1) { //illegal move
                    return FALSE;
                } else {
                    return TRUE;
                }
            }
            case W_KING : { //insert here
                char xDisplacement = (char) (m.xTo - m.xFrom);
                char yDisplacement = (char) (m.yTo - m.yFrom);
                int horizontalSign = ((xDisplacement > 0) * 2) - 1; //gets the sign
                int verticalSign = ((yDisplacement > 0) * 2) - 1;
                int absXDistance = horizontalSign*xDisplacement; //magnitude
                int absYDistance = verticalSign*yDisplacement;
                if (absXDistance == 2) { //castling, potentially split it up
                    if (g->wKingMoved == TRUE) { //we can't castle if the king has already moved
                        return FALSE;
                    }
                    if (absYDistance != 0) {
                        return FALSE;
                    }
                    if (horizontalSign == 1) {//kingside castle
                        if (g->wRightRookMoved == TRUE || g->board[7][0] != W_ROOK) {
                            printf("Debug: The rook has already been moved or taken.\n");
                            return FALSE;
                        }
                        //piece obstruction of the castling linear path
                        if (g->board[m.xFrom + 1][m.yFrom] != EMPTY || g->board[m.xFrom + 2][m.yFrom] != EMPTY ) { //not hardcoded for chess960 possibly in future. Would need amendments
                            return FALSE;
                        }
                        //we need to check if the king is moving through check and disallow this. Done elsewhere
                        return TRUE;
                    } else { //queenside castling
                        if (g->wLeftRookMoved == TRUE || g->board[0][0] != W_ROOK) {
                            printf("Debug: The rook has already been moved or taken.\n");
                            return FALSE;
                        }
                        if (g->board[m.xFrom - 1][m.yFrom] != EMPTY || g->board[m.xFrom - 2][m.yFrom] != EMPTY || g->board[m.xFrom - 3][m.yFrom] != EMPTY ) {
                            return FALSE;
                        }
                        //we need to check if the king is moving through check and disallow this. Done elsewhere
                        return TRUE;
                    }
                } else if (absXDistance > 1 || absYDistance > 1) { //illegal move
                    return FALSE;
                } else {
                    return TRUE;
                }
            }
            default:
                if (!noVerbose) printf("This should never happen lmao.");
                assert(0);
        }

        if (!noVerbose) printf("No value returned for legality\n");
        assert(0);
        return 0;
    }

    void printGame(Game g)
    {
        int i, j;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                //printf("%d\n", g->board[i][j]);
                printf("%c \t", getPieceChar(g->board[j][7-i])); //BE CAREFUL
            }
            printf("\n");
        }
    }

    unsigned char getPieceChar(pieceValue piece)
    {
        //printf("Input: %d\n", piece);
        unsigned char returnVal = ' ';
        switch (piece) {
            case EMPTY: returnVal = '_'; break;
            case W_PAWN: returnVal = 'p'; break;
            case W_KNIGHT: returnVal = 'n'; break;
            case W_BISHOP: returnVal = 'b'; break;
            case W_ROOK: returnVal = 'r'; break;
            case W_QUEEN: returnVal = 'q'; break;
            case W_KING: returnVal = 'k'; break;
            case B_PAWN: returnVal = 'P'; break;
            case B_KNIGHT: returnVal = 'N'; break;
            case B_BISHOP: returnVal = 'B'; break;
            case B_ROOK: returnVal = 'R'; break;
            case B_QUEEN: returnVal = 'Q'; break;
            case B_KING: returnVal = 'K'; break;
            default: printf("lols piece error, piece: %d\n", piece); assert(0);
        }
        return returnVal;
    }

}
