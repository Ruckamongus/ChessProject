#ifndef __ChessBoardHPP__
#define __ChessBoardHPP__ 1
#include <Chess/John/JohnDefinitions.hpp>
#include <Chess/NotationParser.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <string>

class GUIManager;
class NotationParser;

namespace sf
{
    class RenderWindow;
}

class Board
{
    public:
        Board()=default;
       ~Board()=default;
        Board(const Board& other)=delete;
        Board(const Board&& other)=delete;
        Board& operator=(Board other)=delete;
        Board& operator=(Board&& other)=delete;

        void clicked(const Game g, const sf::Vector2i& mousePosition);
        void draw(sf::RenderWindow& Window);
        void linkGUIManager(GUIManager& GUI);
        bool loadResources();
        void reset();
        void update(const Game g, const sf::Vector2i& mousePosition, sf::RenderWindow& Window);
        void getOpponentMove(const Game g, move Move);
        void networkDraw(bool connected, bool serverMove, const std::string& myName, const std::string& opponentName, const sf::Time& myTime, const sf::Time& opponentTime);

        inline move        getNetworkMove()           {auto Return = m_NetworkMove; m_NetworkMove.xFrom = 100; return Return;}
        inline bool        getParsing()         const {return m_ParserMoves.size();}
        inline std::size_t getParsingWaitTime() const {return m_ParserTime;}

        friend class GUIManager;

    private:
        sf::Vector2i      m_Position = sf::Vector2i(16, 16);
        sf::Vector2i      m_SelectPosition = sf::Vector2i(0, 0);
        sf::Texture       m_TexBoard;
        sf::Texture       m_TexPieces;
        sf::Sprite        m_SprBoard[4];
        sf::Sprite        m_SprPieces[12];
        sf::RenderTexture m_TexCompleteBoard;
        sf::RenderTexture m_TexPiecesSurface;
        sf::Font          m_Font;

        bool              m_RequiresRedraw = 1;
        bool              m_Selected = 0;
        bool              m_EnforceTouchMove = 0;
        bool              m_VerboseLogging = 0;
        std::size_t       m_BoardSquareSize = 64;//Pixels of each piece and square
        std::size_t       m_CurrentMove = 1;//Increments each move pair
        move              m_NetworkMove = {100, 100, 0, 0};

        GUIManager*       m_GUIManager = nullptr;
        NotationParser    m_Parser;
        Game              m_ParserCache = nullptr;
        std::queue<move>  m_ParserMoves;
        std::size_t       m_ParserTime = 10;//ms

        bool              m_ServerMove;
        bool              m_Connected = 0;
        std::string       m_OpponentName;
        std::string       m_MyName;
        sf::Time          m_MyTime;
        sf::Time          m_OpponentTime;


        void            refreshBoard(const Game g);
        void            runParser(const std::string& String, const std::string& Time);
        void            reportAndMove(const Game g, move m);
        sf::Vector2i    getMouseCell(const sf::Vector2i& mousePosition) const;
        };

#endif
