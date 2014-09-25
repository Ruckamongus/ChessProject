#ifndef __ChessGUIManagerHPP__
#define __ChessGUIManagerHPP__ 1
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

class Board;

class GUIManager
{
    public:
        bool init(sf::RenderWindow& Window);
        void draw();
        void update(sf::Event& event);
        void addText(const std::string& Text);
        void linkBoard(Board& board);
        void doCallbacks();
        void reset();

        inline std::size_t getNewGame()
        {
            if (m_NewGame)
            {
                std::size_t R = m_NewGame;
                m_NewGame = 0;
                return R;
            }
            return 0;
        }

    private:
        tgui::Gui m_GUI;
        tgui::TextBox::Ptr  m_OutputBox = tgui::TextBox::Ptr(m_GUI);
        tgui::TextBox::Ptr  m_InputBox = tgui::TextBox::Ptr(m_GUI);
        tgui::EditBox::Ptr  m_TimeBox = tgui::EditBox::Ptr(m_GUI);
        tgui::Checkbox::Ptr m_EnforceTouchMove = tgui::Checkbox::Ptr(m_GUI);
        tgui::Checkbox::Ptr m_VerboseLogging = tgui::Checkbox::Ptr(m_GUI);
        tgui::Checkbox::Ptr m_960Board = tgui::Checkbox::Ptr(m_GUI);
        tgui::Button::Ptr   m_RenewBoard = tgui::Button::Ptr(m_GUI);
        tgui::Button::Ptr   m_RunInput = tgui::Button::Ptr(m_GUI);

        std::size_t m_NewGame = 0;

        Board* m_Board = nullptr;
};

#endif
