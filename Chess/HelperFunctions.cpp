#include <Chess/HelperFunctions.hpp>
#include <Phox/Utilities/Conversions.hpp>

sf::Vector2i cellCoordFlip(const sf::Vector2i& cell)
{
    //Flip the coordinate system between standard and chess defined
    return sf::Vector2i(cell.x, 7 - cell.y);
}

std::string stringUpper(const std::string& String)
{
    std::string Return("");

    for (std::size_t i = 0; i < String.size(); ++i)
    {
        char Char = String[i];

        if (Char >= 97 && Char <= 122)
        {
            Return += Char ^ 0x20;
        }
        else
        {
            Return += Char;
        }
    }
    return Return;
}

std::string getPieceName(pieceValue piece)
{
    const char S = stringUpper(Phox::toString(piece))[0];
    switch (S)
    {
        case 'P': return "Pawn";
        case 'N': return "Knight";
        case 'B': return "Bishop";
        case 'R': return "Rook";
        case 'Q': return "Queen";
        case 'K': return "King";
        default:  return "NULL";
    }
}

unsigned char numberToLetterCoords(char number)
{
    switch (number)
    {
        case 0:  return 'a';
        case 1:  return 'b';
        case 2:  return 'c';
        case 3:  return 'd';
        case 4:  return 'e';
        case 5:  return 'f';
        case 6:  return 'g';
        case 7:  return 'h';
        default: return 'a';
    }
}

unsigned char letterToReal(char number)
{
    switch (number)
    {
        case '0':  return 0;
        case '1':  return 1;
        case '2':  return 2;
        case '3':  return 3;
        case '4':  return 4;
        case '5':  return 5;
        case '6':  return 6;
        case '7':  return 7;
        default:   return 0;
    }
}

unsigned char letterToNumberCoords(char letter)
{
    switch (letter)
    {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'g': return 6;
        case 'h': return 7;
        default:  return 0;
    }
}
