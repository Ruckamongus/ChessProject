#ifndef __HelperFunctionsHPP__
#define __HelperFunctionsHPP__
#include <SFML/System/Vector2.hpp>
#include <Chess/John/JohnDefinitions.hpp>
#include <string>

inline std::string turnAsString(int turn)
{
    if (turn == COLOR_WHITE)
    {
        return "White";
    }
    return "Black";
}

inline int otherMove(int Move)
{
    if (Move == COLOR_BLACK)
    {
        return COLOR_WHITE;
    }
    return COLOR_BLACK;
}

sf::Vector2i cellCoordFlip(const sf::Vector2i& cell);
std::string  stringUpper(const std::string& String);
std::string  getPieceName(pieceValue piece);
char         numberToLetterCoords(char number);
char         letterToNumberCoords(char letter);

#endif
