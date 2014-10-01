#ifndef __JohnDefinitions__
#define __JohnDefinitions__
extern "C"
{
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>

	#define COLOR_NONE  0
	#define COLOR_BLACK 1
	#define COLOR_WHITE 2

	#define EMPTY       0
	#define B_PAWN      1
	#define B_KNIGHT    2
	#define B_BISHOP    3
	#define B_ROOK      4
	#define B_QUEEN     5
	#define B_KING      6
	#define W_PAWN      7
	#define W_KNIGHT    8
	#define W_BISHOP    9
	#define W_ROOK      10
	#define W_QUEEN     11
	#define W_KING      12
	#define FALSE       0
	#define TRUE        1
	#define NO_ENPASSANT 10

	typedef unsigned char pieceValue;
	typedef unsigned char boardCoord;
	typedef unsigned char emptyBlackWhite; //value of a board coordinate

	typedef struct _game {
		pieceValue board[8][8];
		emptyBlackWhite whoseMove; //COLOR_BLACK or COLOR_WHITE
		boardCoord canEnPassant; //if not NO_ENPASSANT (10), the X value (file) we can enpassant on
		bool wKingMoved;
		bool bKingMoved;
	} game;

	typedef struct _move {
		boardCoord xTo, xFrom, yTo, yFrom;
	} move;

	typedef game* Game;



    void            printGame(Game g);
    unsigned char   getPieceChar(pieceValue piece);
    Game            newGame();
    void            deleteGame(Game g);
    int             makeMove(const Game g, move m, bool noVerbose = 0); //returns if the move was made
    int             doMove(const Game g, move m, bool noVebose = 0);
    int             isMoveLegal(const Game g, move m, bool noVerbose = 0);
    double          evaluatePosition(game g); //do not pass by reference perhaps? Think later.
    move            getInput(); //function with a side effect
    int             isInCheck(const Game g, emptyBlackWhite colorChecking);
    emptyBlackWhite getSquare(pieceValue piece);

    bool            intIsOdd(size_t Int);
    pieceValue      charToPiece960(char Char, bool White);
    void            randomize960(Game g);
}

#endif
