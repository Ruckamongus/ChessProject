#include <Chess\John\JohnDefinitions.hpp>

extern "C"
{
    double evaluatePosition(const Game g) {
        double score;
        int i, j;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (g->board[i][j] == W_PAWN) {
                    score += 1;
                } else if (g->board[i][j] == B_PAWN) {
                    score -= 1;
                } else if (g->board[i][j] == W_KNIGHT) {
                    score += 3;
                } else if (g->board[i][j] == B_KNIGHT) {
                    score -= 3;
                } else if (g->board[i][j] == W_BISHOP) {
                    score += 3;
                } else if (g->board[i][j] == B_BISHOP) {
                    score -= 3;
                } else if (g->board[i][j] == W_ROOK) {
                    score += 5;
                } else if (g->board[i][j] == B_ROOK) {
                    score -= 5;
                } else if (g->board[i][j] == W_QUEEN) {
                    score += 9;
                } else if (g->board[i][j] == B_QUEEN) {
                    score -= 9;
                }
            }
        }

        return score;
    }
}
