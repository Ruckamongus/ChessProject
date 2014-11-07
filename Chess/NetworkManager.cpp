#include <Chess/NetworkManager.hpp>
#include <Chess/John/JohnDefinitions.hpp>
#include <Phox/Utilities/Conversions.hpp>
#include <iostream>
#include <cstring>

namespace
{
    Phox::cStreamBuffer& operator<<(Phox::cStreamBuffer& Stream, const move& Move)
    {
        Stream << Move.xTo << Move.xFrom << Move.yTo << Move.yFrom;
        return Stream;
    }

    Phox::cStreamBuffer& operator>>(Phox::cStreamBuffer& Stream, move& Move)
    {
        Stream >> Move.xTo >> Move.xFrom >> Move.yTo >> Move.yFrom;
        return Stream;
    }

    Phox::cStreamBuffer& operator<<(Phox::cStreamBuffer& Stream, const _game g)
    {
        Stream.write(&g, sizeof(_game));
        return Stream;
    }

    Phox::cStreamBuffer& operator>>(Phox::cStreamBuffer& Stream, _game& g)
    {
        std::memcpy(&g, Stream.getConstPointer() + Stream.getReadPosition(), sizeof(_game));
        return Stream;
    }
}

NetworkManager::NetworkManager()
{
    m_Listener.setBlocking(0);
    m_Socket.setBlocking(0);
}

void NetworkManager::getMove(const move& Move)
{
    if (Move.xFrom == 100) return;//Nothing to report here
    if (m_Connected)
    {
        m_WhitesMove = !m_WhitesMove;
        m_MyTime += m_Clock.getElapsedTime().asMilliseconds();
        m_Clock.restart();
        Phox::cStreamBuffer buff;
        buff.writeUnsignedByte(2);
        buff << Move;
        m_Socket.send(buff.getConstPointer(), buff.getWorkingBytes());
    }
}

void NetworkManager::setCurrentBoard(Game g)
{
    std::memcpy(&m_Board, g, sizeof(game));
    refreshBoard(&m_Board);
    m_RequestBoard = 0;
}

void NetworkManager::refreshBoard(Game g)
{
    Phox::cStreamBuffer buff;
    buff.writeUnsignedByte(3);
    buff << *g;
    buff.writeUnsignedByte(m_WhitesMove);
    m_Socket.send(buff.getConstPointer(), buff.getWorkingBytes());
}

void NetworkManager::startGame()
{
    m_Clock.restart();
    m_WhitesMove = 1;
}

void NetworkManager::doNetworkStuff()
{
    if (!m_Connected)//Listen to new connection
    {
        if (m_Hosting && m_Listener.accept(m_Socket) == sf::Socket::Done)
        {
            m_Connected = 1;
            m_RequestBoard = 1;
            Phox::cStreamBuffer buff;
            buff.writeUnsignedByte(0);
            buff.writeString(m_MyName);
            m_Socket.send(buff.getConstPointer(), buff.getWorkingBytes());
        }
    }

    else

    {
        char Data[512];
        std::size_t Size;
        sf::Socket::Status status = m_Socket.receive(Data, 512, Size);

        if (Size)
        {
            if (status == sf::Socket::Done)
            {
                Phox::cStreamBuffer Buffer;
                Buffer.write(&Data, Size);

                switch (Buffer.readUnsignedByte())
                {
                    case 0://Login name exchange
                    {
                        m_Signal.clear();
                        m_OpponentName = Buffer.readString();
                        if (m_Hosting)
                        {
                            m_Signal << (m_OpponentName + " has connected.\n");
                        }

                        else

                        {
                            m_Signal << (std::string("Connected to ") + m_OpponentName + ".\n");
                        }
                        startGame();
                    }
                    break;

                    case 1://Chat
                        m_Signal.clear();
                        m_Signal.writeString(m_OpponentName + ": " + Buffer.readString() + '\n');
                        break;

                    case 2://New move
                        Buffer >> m_ReportMove;
                        m_OpponentTime += m_Clock.getElapsedTime().asMilliseconds();
                        m_Clock.restart();
                        m_WhitesMove = !m_WhitesMove;
                    break;

                    case 3://New board
                        Buffer >> m_Board;
                        m_WhitesMove = Buffer.readUnsignedByte();
                        m_RefreshBoard = 1;
                        m_Clock.restart();
                        m_MyTime = 0;
                        m_OpponentTime = 0;
                        m_Signal.clear();
                        m_Signal << "_clear_\1";
                        m_Signal.writeString(m_OpponentName);
                        m_Signal.writeUnsignedInt(m_MyTime);
                        m_Signal.writeUnsignedInt(m_OpponentTime);
                        break;

                    default: break;
                }
            }
        }

        if (status == sf::Socket::Disconnected)
        {
            m_Signal.clear();
            m_Signal << "_clear_\2";
            m_Signal.writeString(m_OpponentName);
            m_Signal.writeUnsignedInt(m_MyTime);
            m_Signal.writeUnsignedInt(m_OpponentTime);
            m_Signal.writeString(m_OpponentName + " has disconnected.\n");
            m_Connected = 0;
            m_Socket.disconnect();
        }

        if (status == sf::Socket::Error)
        {
            std::cout << "[Socket] Error.\n";
        }
    }
}

void NetworkManager::handleSignal(Phox::cStreamBuffer Signal)
{
    doNetworkStuff();

    if (!Signal.getWorkingBytes()) return;//Nothing to do here

    std::string Sig;
    Signal >> Sig;

    if (Sig == "Connect")
    {
        m_Socket.disconnect();
        m_Hosting = 0;

        std::string User, IP, Port;
        Signal >> User >> IP >> Port;
        m_MyName = User;

        m_Socket.setBlocking(1);
        sf::Socket::Status status = m_Socket.connect(IP, static_cast <unsigned short> (Phox::ToDouble(Port)), sf::seconds(5.f));
        m_Socket.setBlocking(0);

        if (status != sf::Socket::Done)
        {
            std::string Err = "Cannot connect to host.\n";

            if (status == sf::Socket::Error) std::cout << "Socket::Error\n";
            if (status == sf::Socket::Disconnected) std::cout << "Socket::DC\n";
            if (status == sf::Socket::NotReady) std::cout << "Socket::NotReady\n";

            m_Signal.clear();
            m_Signal << "Error";
            m_Signal << Err;
        }

        else

        {
            m_Connected = 1;
            Phox::cStreamBuffer buff;
            buff.writeUnsignedByte(0);
            buff.writeString(User);
            m_Socket.send(buff.getConstPointer(), buff.getWorkingBytes());
        }
    }

    else if (Sig == "Host")
    {
        unsigned short Port = static_cast<unsigned short> (Phox::ToDouble(Signal.readString()));
        Signal >> m_MyName;
        m_EnforceTouchMove = Signal.readUnsignedByte();

        if (m_Listener.listen(Port) != sf::Socket::Done)
        {
            std::string Err = "Cannot listen to port ";
            Err += Phox::toString(Port);

            if (m_Hosting)
            {
                Err += "; already listening.\n";
            }

            else Err += ".\n";

            std::cerr << Err;

            m_Signal.clear();
            m_Signal << "Error";
            m_Signal << Err;
        }
        else
        {
            m_Connected = 0;
            m_Hosting = 1;

            std::string Str = "Listening to port ";
            Str += Phox::toString(Port) + ".\n";

            m_Signal.clear();
            m_Signal << Str;
        }
    }

    else if (Sig == "Forfeit")
    {

    }

    else if (Sig == "Rematch")
    {

    }

    else if (Sig == "Chat")
    {
        if (m_Connected)
        {
            Phox::cStreamBuffer buff;
            buff.writeUnsignedByte(1);
            buff.writeString(Signal.readString());
            m_Socket.send(buff.getConstPointer(), buff.getWorkingBytes());
        }
    }
}

Phox::cStreamBuffer NetworkManager::getGUISignal()
{
    auto Return = m_Signal;
    m_Signal.clear();
    return Return;
}
