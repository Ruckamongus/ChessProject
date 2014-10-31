#ifndef __ChessNetworkManager__
#define __ChessNetworkManager__ 1
#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <Phox/Utilities/StreamBuffer.hpp>
#include <Chess/John/JohnDefinitions.hpp>
#include <string>

class NetworkManager
{
    public:
        NetworkManager();
       ~NetworkManager()=default;
        NetworkManager(const NetworkManager& other)=delete;
        NetworkManager(const NetworkManager&& other)=delete;
        NetworkManager& operator=(NetworkManager other)=delete;
        NetworkManager& operator=(NetworkManager&& other)=delete;

        void getMove(const move& Move);
        void handleSignal(Phox::cStreamBuffer Signal);
        void refreshBoard(Game g);
        void setCurrentBoard(Game g);
        Phox::cStreamBuffer getGUISignal();

        inline bool isHosting()    const {return m_Hosting;}
        inline bool isConnected()  const {return m_Connected;}
        inline bool isWhitesMove() const {return m_WhitesMove;}
        inline bool isBoardFresh() const {return m_RefreshBoard;}
        inline bool requestCurrentBoard() const {return m_RequestBoard;}
        inline move reportMove()         {auto Return = m_ReportMove; m_ReportMove.xFrom = 100; return Return;}
        inline Game getRefreshedBoard()  {m_RefreshBoard = 0; return &m_Board;}

    private:
        sf::Clock           m_ClockWhite;
        sf::Clock           m_ClockBlack;
        sf::TcpListener     m_Listener;
        sf::TcpSocket       m_Socket;
        Phox::cStreamBuffer m_Signal;
        std::string         m_OpponentName;
        std::string         m_MyName;
        bool                m_Connected = 0;
        bool                m_Hosting = 0;
        bool                m_WhitesMove = 1;
        bool                m_EnforceTouchMove = 0;
        bool                m_RefreshBoard = 0;
        bool                m_RequestBoard = 0;
        _game               m_Board;
        move                m_ReportMove = {100, 0, 0, 0};

        void doNetworkStuff();
        void startGame();
};
#endif
