#ifndef __ChessNetworkManager__
#define __ChessNetworkManager__ 1
#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <Phox/Utilities/StreamBuffer.hpp>
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

        void handleSignal(Phox::cStreamBuffer Signal);
        Phox::cStreamBuffer getGUISignal();
        inline bool isHosting() const {return m_Hosting;}
        inline bool isConnected() const {return m_Connected;}

    private:
        sf::Clock           m_Clock;
        sf::TcpListener     m_Listener;
        sf::TcpSocket       m_Socket;
        Phox::cStreamBuffer m_Signal;
        std::string         m_OpponentName;
        std::string         m_MyName;
        bool                m_Connected = 0;
        bool                m_Hosting = 0;

        void doNetworkStuff();
};
#endif
