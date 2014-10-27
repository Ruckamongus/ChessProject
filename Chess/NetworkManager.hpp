#ifndef __ChessNetworkManager__
#define __ChessNetworkManager__ 1
#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <string>

class NetworkManager
{
    public:
        NetworkManager()=default;
       ~NetworkManager()=default;
        NetworkManager(const NetworkManager& other)=delete;
        NetworkManager(const NetworkManager&& other)=delete;
        NetworkManager& operator=(NetworkManager other)=delete;
        NetworkManager& operator=(NetworkManager&& other)=delete;

        void handleSignal(const std::string& Signal);
        inline std::string getSignal() const {return m_Signal;}
        inline bool isHosting() const {return m_Hosting;}
        inline bool isConnected() const {return m_Connected;}

    private:
        sf::Clock       m_Clock;
        sf::TcpListener m_Listener;
        sf::TcpSocket   m_Socket;
        sf::IpAddress   m_IP;
        std::string     m_Signal;
        bool            m_Connected = 0;
        bool            m_Hosting = 0;

        void setUp(sf::IpAddress IP, unsigned short Port);
};
#endif
