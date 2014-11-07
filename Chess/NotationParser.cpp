#include <Chess/NotationParser.hpp>
#include <Chess/HelperFunctions.hpp>
#include <Phox/Utilities/Conversions.hpp>
#include <sstream>
#include <iostream>

namespace
{
    std::string stringReplaceAll(const std::string& str, const std::string& subStr, const std::string& rep)
    {
        std::string Ret = str;
        auto pos = Ret.find(subStr);

        while (pos != std::string::npos)
        {
            Ret.erase(Ret.begin() + pos, Ret.begin() + pos + subStr.size());
            Ret.insert(pos, rep);
            pos = Ret.find(subStr);
        }

        return Ret;
    }

    std::string stringDigitsPeriodOnly(const std::string& String)
    {
        std::string Return("");
        for (unsigned int i = 0; i < String.size(); i++)
        {
            char Char = String[i];
            if ((Char >= 48 && Char <= 57) || (Char == '.'))
                Return += Char;
        }
        return Return;
    }

    bool containsLettersAndNumbers(const std::string& String)
    {
        bool Letters = 0;
        bool Numbers = 0;

        for (unsigned int i = 0; i < String.size(); i++)
        {
            char Char = String[i];
            if (!Letters)
            {
                if ((Char >= 48 && Char <= 57) || (Char == ':') || (Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z'))
                    Letters = 1;
            }

            if (!Numbers)
            {
                if (Char >= '0' && Char <= '9')
                    Numbers = 1;
            }

            if (Letters && Numbers)
                return 1;
        }
        return Letters && Numbers;
    }

    bool isMove(const std::string& String)
    {
        std::string Return("");
        for (unsigned int i = 0; i < String.size(); i++)
        {
            char Char = String[i];
            if ((Char >= 48 && Char <= 57) || (Char == ':') || (Char >= 'A' && Char <= 'Z') || (Char >= 'a' && Char <= 'z'))
                Return += Char;
        }

        if (!containsLettersAndNumbers(Return)) return 0;
        return Return == String;
    }

    std::string makeErrorHeader(std::size_t Number, std::string Move, emptyBlackWhite Who)
    {
        return std::string("Move pair ") + Phox::toString(Number)
             + std::string(" \"") + Move + std::string("\"\n\t")
             + std::string(Who==COLOR_WHITE?"White ":"Black ");
    }

    std::ostream& operator<< (std::ostream& o, const move& m)
    {
        o << "From (" << (int)m.xFrom << ", " << (int)m.yFrom << ") to (" << (int)m.xTo << ", " << (int)m.yTo << ")\n";
    }
}

void NotationParser::setString(const std::string& String)
{
    m_RawString = String;
}

void NotationParser::setBoard(game g)//Yes, copy it in.
{
    m_Board = g;
}

int NotationParser::parse()
{
    while (m_RawQueue.size())  m_RawQueue.pop();
    while (m_MoveQueue.size()) m_MoveQueue.pop();
    m_ErrorString = "";

    std::string In("");
    std::stringstream ss;
    ss << stringReplaceAll(m_RawString, "\n", " ");

    while (ss >> In)
    {
        if (isMove(In))
        {
            m_RawQueue.push(In);
        }
    }

    std::size_t MoveNumber = 1;

    while (m_RawQueue.size())
    {
        std::string MoveStr = stringReplaceAll(m_RawQueue.front(), "x", "");
        MoveStr = stringReplaceAll(MoveStr, "+", "");//Remove check notation
        MoveStr = stringReplaceAll(MoveStr, ":", "");
        MoveStr = stringReplaceAll(MoveStr, "0-0", "");//Remove queenside castling
        MoveStr = stringReplaceAll(MoveStr, "0-0-0", "");//Remove queenside castling

        bool Capture = (MoveStr != m_RawQueue.front());
        bool Invalid = 1;
        //Removed "captured notation" leaving bare move

        std::string PieceKind = getPieceName(stringUpper(MoveStr)[0]);
        if (PieceKind == "NULL")
        {
            PieceKind = "Pawn";
        }
        else
        {
            MoveStr.erase(0, 1);//Remove the front letter designating piece kind
        }

        unsigned char Column = letterToNumberCoords(MoveStr[MoveStr.size()-2]);
        unsigned char Row = letterToReal(MoveStr[MoveStr.size()-1]) - 1;

        if (PieceKind == "Pawn")
        {
            if (!Capture)
            {
                for (std::size_t i = 0; i < 8; ++i)//Loop through column on board
                {
                    if (m_Board.board[Column][i] == (m_Board.whoseMove==COLOR_WHITE?W_PAWN:B_PAWN))
                    {
                        //boardCoord xTo, xFrom, yTo, yFrom;
                        move test{Column, Column, Row, static_cast<boardCoord> (i)};
                        if (isMoveLegal(&m_Board, test, 1))
                        {
                            makeMove(&m_Board, test, 1);
                            m_MoveQueue.push(test);
                            Invalid = 0;
                        }
                    }
                }
                if (Invalid)
                {
                    m_ErrorString = makeErrorHeader((MoveNumber >> 1) + 1, m_RawQueue.front(), m_Board.whoseMove)
                                  + std::string("Pawn does not have valid non-capture move as indicated.");
                    return 0;
                }
            }
            else//Capture
            {
                for (std::size_t i = 0; i < 8; ++i)//Loop through column on board for pawn
                {
                    if (m_Board.board[letterToNumberCoords(MoveStr[0])][i] == (m_Board.whoseMove==COLOR_WHITE?W_PAWN:B_PAWN))
                    {
                        move test{Column, letterToNumberCoords(MoveStr[0]), Row, static_cast<boardCoord> (i)};
                        if (isMoveLegal(&m_Board, test, 1))
                        {
                            makeMove(&m_Board, test, 1);
                            m_MoveQueue.push(test);
                            Invalid = 0;
                        }
                    }
                }
                if (Invalid)
                {
                    m_ErrorString = makeErrorHeader((MoveNumber >> 1) + 1, m_RawQueue.front(), m_Board.whoseMove)
                                  + std::string("Pawn does not have valid capture move as indicated.");
                    return 0;
                }
            }
        }
        //1. g4 f5
        //2. gxf5   ->    gf5     ->      f5

        else//Non-pawn too.move
        {

        }


		//boardCoord xTo, xFrom, yTo, yFrom;

        MoveNumber++;
        m_RawQueue.pop();
    }
    return 1;
}
