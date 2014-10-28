#ifndef __ChessGUIManagerHPP__
#define __ChessGUIManagerHPP__ 1
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <Phox/Utilities/StreamBuffer.hpp>

class Board;

class GUIManager
{
    public:
        GUIManager()=default;
       ~GUIManager()=default;
        GUIManager(const GUIManager& other)=delete;
        GUIManager(const GUIManager&& other)=delete;
        GUIManager& operator=(GUIManager other)=delete;
        GUIManager& operator=(GUIManager&& other)=delete;

        void addText(const std::string& Text);
        void draw();
        void init(sf::RenderWindow& Window);
        void linkBoard(Board& board);
        void reset();
        void update(sf::Event& event);
        void handleSignal(Phox::cStreamBuffer Signal);

        inline bool getTouchMove()      const {return m_EnforceTouchMove->isChecked();}
        inline bool get960Board()       const {return m_960Board->isChecked();}
        inline bool getVerboseLogging() const {return m_VerboseLogging->isChecked();}

        std::size_t getNewGame();
        Phox::cStreamBuffer getNetworkSignal();

    private:
        tgui::Gui           m_GUI;
        tgui::TextBox::Ptr  m_OutputBox;
        tgui::TextBox::Ptr  m_InputBox;
        tgui::EditBox::Ptr  m_TimeBox;
        tgui::Checkbox::Ptr m_EnforceTouchMove;
        tgui::Checkbox::Ptr m_VerboseLogging;
        tgui::Checkbox::Ptr m_960Board;
        tgui::Button::Ptr   m_RenewBoard;
        tgui::Button::Ptr   m_RunInput;

        tgui::TextBox::Ptr  m_NetworkChatbox;
        tgui::EditBox::Ptr  m_NetworkUser;
        tgui::EditBox::Ptr  m_NetworkPort;
        tgui::EditBox::Ptr  m_NetworkIP;
        tgui::Button::Ptr   m_NetworkHost;
        tgui::Button::Ptr   m_NetworkConnect;
        tgui::Button::Ptr   m_NetworkForfeit;
        tgui::Button::Ptr   m_NetworkRematch;
        tgui::EditBox::Ptr  m_NetworkChat;

        std::size_t m_NewGame = 0;
        Phox::cStreamBuffer m_NetworkSignal;

        Board* m_Board = nullptr;
};

#endif
