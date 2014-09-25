#include <Chess/Board.hpp>
#include <Phox/Internal/Exception.hpp>
#include <Phox/Internal/IODevices.hpp>
#include <Phox/Utilities/Conversions.hpp>
#include <Chess/GUIManager.hpp>
#include <Chess/HelperFunctions.hpp>
#include <Chess/NotationParser.hpp>
#include <iostream>
namespace Phox
{
    namespace Internal
    {
        extern sf::RenderWindow* Window;//Access to the game's window
    }
}

namespace
{
    inline bool isOdd(unsigned int i)
    {
        return  (i % 2 == 0);
    }

    inline bool isWhitePiece(pieceValue piece)
    {
        return (piece > 6 && piece < 13);
    }

    inline int pieceToArrayIndex(pieceValue piece)
    {
        //Get the sprite's array index of the given piece
        switch (piece)
        {
            case EMPTY:    return -1;
            case B_PAWN:   return 6;
            case B_KNIGHT: return 8;
            case B_BISHOP: return 7;
            case B_ROOK:   return 9;
            case B_QUEEN:  return 10;
            case B_KING:   return 11;

            case W_PAWN:   return 0;
            case W_KNIGHT: return 2;
            case W_BISHOP: return 1;
            case W_ROOK:   return 3;
            case W_QUEEN:  return 4;
            case W_KING:   return 5;
            default: return 0;
        }
    }

    std::string getMoveData(const Game g, const move& m, bool Captured)
    {
        //Assumes a valid move
        std::string Str = "";

        std::string Piece;
        Piece += getPieceChar(g->board[m.xTo][m.yTo]);
        Piece = stringUpper(Piece);
        if (Piece == "P")
        {
            Piece = "";
            if (Captured)
                Str += Phox::toString(numberToLetterCoords(m.xFrom));
        }

        Str += Piece;
        if (Captured) Str+= "x";
        Str += numberToLetterCoords(m.xTo);
        Str += Phox::toString(static_cast<int> (m.yTo + 1));

        return Str;
    }
}

Board::Board()
{
    m_Parser = new (std::nothrow) NotationParser;
    if (m_Parser == nullptr)
    {
        Phox::ThrowException("Could not allocate the input parser.", 1);
    }
}

Board::~Board()
{
    if (m_Parser != nullptr)
    {
        delete m_Parser;
        m_Parser = nullptr;
    }
}


void Board::linkGUIManager(GUIManager& GUI)
{
    m_GUIManager = &GUI;
    GUI.linkBoard(*this);
}

bool Board::loadResources()
{
    if (!m_TexBoard.loadFromFile("Resources\\BoardSquares.png"))
    {
        Phox::ThrowException("Cannot load \"Resources\\BoardSquares.png\".");
        return 0;
    }

    if (!m_TexPieces.loadFromFile("Resources\\BoardPieces.png"))
    {
        Phox::ThrowException("Cannot load \"Resources\\BoardPieces.png\".");
        return 0;
    }

    //Load the board squares sprites
    for (std::size_t i = 0; i < 4; ++i)
    {
        m_SprBoard[i].setTexture(m_TexBoard);
        m_SprBoard[i].setTextureRect(sf::IntRect(i * m_BoardSquareSize, 0, m_BoardSquareSize, m_BoardSquareSize));
    }

    //Load the pieces sprites
    for (std::size_t i = 0; i < 12; ++i)
    {
        m_SprPieces[i].setTexture(m_TexPieces);
        m_SprPieces[i].setTextureRect(sf::IntRect(i * m_BoardSquareSize - (i>5?6*m_BoardSquareSize:0), m_BoardSquareSize * (i > 5), m_BoardSquareSize, m_BoardSquareSize + m_BoardSquareSize * (i > 5)));
    }

    //Create the board and drawing surfaces
    std::size_t Error = 0;
    Error += m_TexPiecesSurface.create(m_BoardSquareSize * 8, m_BoardSquareSize * 8);
    m_TexPiecesSurface.setSmooth(1);
    m_TexPiecesSurface.clear(sf::Color::Transparent);

    Error += m_TexCompleteBoard.create(m_BoardSquareSize * 8, m_BoardSquareSize * 8);
    m_TexCompleteBoard.setSmooth(0);
    m_TexCompleteBoard.clear();

    Error += m_Font.loadFromFile("Resources\\DejaVuSans.ttf");

    if (Error < 3)
    {
        Phox::ThrowException("[Fatal]Could not allocate render surfaces or find the font file.", 1);
    }

    //Draw the board squares to the board surface
    for (std::size_t i = 0; i < 8; ++i)
    {
        for (std::size_t j = 0; j < 8; ++j)
        {
            std::size_t index = isOdd(i + j);
            m_SprBoard[index].setPosition(m_BoardSquareSize * i, m_BoardSquareSize * j);
            m_TexCompleteBoard.draw(m_SprBoard[index]);
        }
    }
    m_TexCompleteBoard.display();//Finalize the board surface

    return 1;
}

sf::Vector2i Board::getMouseCell() const
{
    //Returns the real cell coordinate. Ex: (0, 0) is top left and (7, 7)is bottom right
    if (sf::IntRect(m_Position, sf::Vector2i(m_BoardSquareSize * 8, m_BoardSquareSize * 8)).contains(Phox::Mouse.x, Phox::Mouse.y))
    {
        return sf::Vector2i((Phox::Mouse.x - m_Position.x) / 64, (Phox::Mouse.y - m_Position.y) / 64);
    }
    return sf::Vector2i(-1, -1);
}

void Board::clicked(const Game g)
{
    //Board was clicked, lets figure out what to do
    sf::Vector2i cell = cellCoordFlip(getMouseCell());
    if (m_Selected)
    {
        std::size_t X, Y;
        if (cell.x > -1)//Clicked within the board
        {
            X = cell.x;
            Y = cell.y;
            sf::Vector2i selectFlipped = cellCoordFlip(m_SelectPosition);

            if (!m_EnforceTouchMove)//Select new piece if not empty spot
            {
                if (cell == selectFlipped)//Selected the same piece to deselect
                {
                    m_Selected = 0;
                    return;
                }

                pieceValue piece = g->board[cell.x][cell.y];
                if (piece != EMPTY)
                {
                    if (isWhitePiece(piece) == g->whoseMove - 1)//If it's the correct color's turn
                    {
                        m_SelectPosition = cellCoordFlip(cell);
                        return;
                    }
                }
            }

            move m = {static_cast<boardCoord> (X), static_cast<boardCoord> (selectFlipped.x), static_cast<boardCoord> (Y), static_cast<boardCoord> (selectFlipped.y)};
            if (isMoveLegal(g, m))
            {
                bool TakesPiece = (g->board[m.xTo][m.yTo] != EMPTY);
                pieceValue Took = getPieceChar(g->board[m.xTo][m.yTo]);
                m_Selected = 0;
                if (makeMove(g, m))//Make move if we can
                {
                    m_RequiresRedraw = 1;//Tell the board it has to update since there was a move
                    if (m_GUIManager != nullptr)
                    {
                        std::string Str = getMoveData(g, m, TakesPiece);
                        std::string Verbose = "";

                        if (m_VerboseLogging)
                        {
                            if (TakesPiece)
                            {
                                Verbose += " (" + turnAsString(otherMove(g->whoseMove)) + " takes " + turnAsString(g->whoseMove) + "'s ";
                                Verbose += getPieceName(Took);
                                if (isInCheck(g, g->whoseMove))
                                {
                                    Verbose += "; puts " + turnAsString(g->whoseMove) + " in check.)";
                                }
                                else Verbose += ".) ";

                                if (g->whoseMove == COLOR_BLACK)//White just went
                                {
                                    Verbose += "\n" + Phox::toString(m_CurrentMove) + "... ";
                                }
                            }

                            else

                            if (isInCheck(g, g->whoseMove))
                            {
                                Verbose += " (" + turnAsString(otherMove(g->whoseMove)) + " puts " + turnAsString(g->whoseMove) + " in check.) ";
                            }
                        }
                        if (g->whoseMove == COLOR_BLACK)//Actually white, lol
                            m_GUIManager->addText(Phox::toString(m_CurrentMove) + ". " + Str + " " + Verbose);
                        else
                        {
                            m_GUIManager->addText(Str + Verbose + '\n');
                            m_CurrentMove++;
                        }
                    }
                }
            }
        }
        else
        {
            if (!m_EnforceTouchMove)
                m_Selected = 0;
        }
    }
    else //Nothing is currently selected
    {
        if (cell.x > -1)//Clicked within the board
        {
            pieceValue piece = g->board[cell.x][cell.y];
            if (piece != EMPTY)
            {
                if (isWhitePiece(piece) == g->whoseMove - 1)//If it's the correct color's turn
                {
                    if (m_EnforceTouchMove)
                    {
                        //This is terribly inefficient; make John fix it later. >= D
                        //We're going to brute force the entire board for a valid move before
                        //locking the piece for touch-move.
                        bool Valid = 0;
                        std::size_t Count = 0;
                        for (std::size_t i = 0; i < 8; ++i)
                        {
                            if (!Valid)
                            for (std::size_t j = 0; j < 8; ++j)
                            {
                                move m = {static_cast<boardCoord> (i), static_cast<boardCoord> (cell.x), static_cast<boardCoord> (j), static_cast<boardCoord> (cell.y)};
                                if (isMoveLegal(g, m))
                                {
                                    Valid = 1;
                                    break;
                                }
                                Count++;
                            }
                        }
                        if (!Valid) return;
                        m_Selected = 1;
                        m_SelectPosition = cellCoordFlip(cell);//Set the selection position
                    }
                    else
                    {
                        m_Selected = 1;
                        m_SelectPosition = cellCoordFlip(cell);//Set the selection position
                    }
                }
            }
        }
    }
}

void Board::update(const Game g)
{
    if (m_RequiresRedraw)//If we should redraw the pieces
    {
        refreshBoard(g);//Redraw them
        #warning Uncomment below when John fixes his shit.
        //m_RequiresRedraw = 0;
    }

    draw();//Draw the board and pieces

    if (m_Selected)//Cell highlighting for selected piece and mouse over cell
    {
        sf::Vector2i Cell = getMouseCell();
        if (Cell.x > -1)
        {
            std::size_t Index = 2;//Defualt to red color
            sf::Vector2i selectFlipped = cellCoordFlip(m_SelectPosition);
            sf::Vector2i cellFlipped = cellCoordFlip(Cell);
            move m = {static_cast<boardCoord> (cellFlipped.x), static_cast<boardCoord> (selectFlipped.x), static_cast<boardCoord> (cellFlipped.y), static_cast<boardCoord> (selectFlipped.y)};
            if (isMoveLegal(g, m))
            {
                Index = 3;//Highlight in green instead of red (3)
            }

            m_SprBoard[Index].setPosition(Cell.x * 64 + m_Position.x, Cell.y * 64 + m_Position.y);
            Phox::Internal::Window->draw(m_SprBoard[Index]);//Draw the mouse-over cell's highlight

            if (Cell != m_SelectPosition)
            {
                m_SprBoard[3].setPosition(m_SelectPosition.x * 64 + m_Position.x, m_SelectPosition.y * 64 + m_Position.y);
                Phox::Internal::Window->draw(m_SprBoard[3]);//Draw the selected piece highlighted in green
            }
        }
    }
    m_ParserCache = g;
}

void Board::draw()
{
    //Draw the surfaces to the window
    sf::RenderWindow& w = *Phox::Internal::Window;
    sf::Sprite spr, spr2;

    spr.setPosition(m_Position.x, m_Position.y);
    spr.setTexture(m_TexCompleteBoard.getTexture());
    w.draw(spr);//Draw the board

    spr2.setPosition(m_Position.x, m_Position.y);
    spr2.setTexture(m_TexPiecesSurface.getTexture());
    w.draw(spr2);//Draw the pieces

    sf::Text t;
    t.setFont(m_Font);
    t.setCharacterSize(14);
    t.setColor(sf::Color::Black);

    for (std::size_t i = 0; i < 8; ++i)
    {
        t.setString(Phox::toString(numberToLetterCoords(i)));
        t.setOrigin(t.getLocalBounds().width/2, 0);//fa_center
        t.setPosition(m_Position.x + (m_BoardSquareSize >> 1) + i * m_BoardSquareSize, m_Position.y - 16);
        w.draw(t);

        t.setString(Phox::toString(8 - i));
        t.setOrigin(0, t.getLocalBounds().height/2);//fa_center
        t.setPosition(m_Position.x - 12, m_Position.y + (m_BoardSquareSize >> 1) + i * m_BoardSquareSize);
        w.draw(t);
    }

    t.setString("Output");
    t.setOrigin(0, 0);
    t.setPosition(544, 14);
    w.draw(t);

    t.setString("Input");
    t.setOrigin(0, 0);
    t.setPosition(544, 230);
    w.draw(t);

    t.setString("Wait time between moves? (ms)");
    t.setOrigin(0, 0);
    t.setPosition(544, 436);
    w.draw(t);
}

void Board::runParser(const std::string& String, const std::string& Time)
{
    if (m_Parser == nullptr || m_ParserCache == nullptr)
    {
        Phox::ThrowException("Parser memory corrupt. Please restart this application.", 0);
        return;
    }

    //sf::Time WaitTime = sf::milliseconds(static_cast<sf::Int32> (Phox::ToDouble(Time)));
    m_Parser->setString(String);
    m_Parser->setBoard(*m_ParserCache);
    m_Parser->parse();
}

void Board::reset()
{
    m_RequiresRedraw = 1;
    m_CurrentMove = 1;
    if (m_GUIManager != nullptr)
    {
        m_GUIManager->reset();
    }
}

void Board::refreshBoard(const Game g)
{
    //Loop through the entire board and draw each piece to the surface
    m_TexPiecesSurface.clear(sf::Color::Transparent);
    for (std::size_t i = 0; i < 8; ++i)
    {
        for (std::size_t j = 0; j < 8; ++j)
        {
            int Index = pieceToArrayIndex(g->board[i][7 - j]);
            if (Index > -1)
            {
                m_SprPieces[Index].setPosition(i * 64, j * 64);
                m_TexPiecesSurface.draw(m_SprPieces[Index]);
            }
        }
    }
    m_TexPiecesSurface.display();//Finalize the texture for drawing
}
