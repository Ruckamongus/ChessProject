#ifndef __ChessBoardHPP__
#define __ChessBoardHPP__ 1
#include <string>
#include <SFML/Graphics.hpp>
#include <Chess/John/JohnDefinitions.hpp>

class GUIManager;
class NotationParser;

class Board
{
    public:
        Board();
        ~Board();
        Board(const Board& other)=delete;
        Board(const Board&& other)=delete;
        Board& operator=(Board other)=delete;
        Board& operator=(Board&& other)=delete;

        void clicked(const Game g);
        void draw();
        bool loadResources();
        void update(const Game g);
        void linkGUIManager(GUIManager& GUI);
        void reset();

        friend class GUIManager;

    private:
        sf::Vector2i m_Position = sf::Vector2i(16, 16);
        sf::Vector2i m_SelectPosition = sf::Vector2i(0, 0);
        sf::Texture  m_TexBoard;
        sf::Texture  m_TexPieces;
        sf::Sprite   m_SprBoard[4];
        sf::Sprite   m_SprPieces[12];
        sf::RenderTexture m_TexCompleteBoard;
        sf::RenderTexture m_TexPiecesSurface;
        sf::Font m_Font;

        unsigned short  m_BoardSquareSize = 64;//Pixels of each piece and square
        bool            m_RequiresRedraw = 1;
        bool            m_Selected = 0;
        bool            m_EnforceTouchMove = 0;
        bool            m_VerboseLogging = 0;
        std::size_t     m_CurrentMove = 1;


        GUIManager*     m_GUIManager = nullptr;
        NotationParser* m_Parser = nullptr;
        Game            m_ParserCache = nullptr;


        void            refreshBoard(const Game g);
        void            runParser(const std::string& String, const std::string& Time);
        sf::Vector2i    getMouseCell() const;
};

#endif
