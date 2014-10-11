#include <Chess/NotationParser.hpp>
#include <Chess/HelperFunctions.hpp>
#include <Phox/Utilities/Conversions.hpp>
#include <sstream>
#include <iostream>

namespace
{
    std::string replaceAllChars(const std::string& String, const char From, const char To)
    {
        std::string Return(String);
        for (unsigned int i = 0; i < String.size(); i++)
        {
            if (Return[i] == From)
                Return[i] = To;
        }
        return Return;
    }

    std::string removeAllChars(const std::string& String, const char Char)
    {
        std::string Return("");
        for (unsigned int i = 0; i < String.size(); i++)
        {
            if (String[i] != Char)
                Return += String[i];
        }
        return Return;
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
    m_ErrorString = "";

    std::string In("");
    std::stringstream ss;
    ss << replaceAllChars(m_RawString, '\n', ' ');

    while (ss >> In)
    {
        if (isMove(In))
        {
            m_RawQueue.push(In);
        }
    }

    bool inValid = 0;

    while (m_RawQueue.size())
    {
        std::string MoveStr = removeAllChars(m_RawQueue.front(), 'x');
        MoveStr = removeAllChars(MoveStr, ':');
        bool Capture = (MoveStr != m_RawQueue.front());
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

        char Row = letterToNumberCoords(MoveStr[0]);
        char Column = MoveStr[1];

        std::cout << MoveStr << '\n';

        if (PieceKind == "Pawn")
        {
            if (!Capture)
            {
                for (std::size_t i = 0; i < 8; ++i)//Loop through column on board
                {
                    //if (m_Board.board[])

                }
            }
            //move One {}
        }
        //1. g4 f5
        //2. gxf5   ->    gf5     ->      f5

        else//Non-pawn too.move
        {

        }


		//boardCoord xTo, xFrom, yTo, yFrom;


        m_RawQueue.pop();

        return 1;
    }
}
