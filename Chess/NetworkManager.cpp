#include <Chess/NetworkManager.hpp>
#include <Chess/John/JohnDefinitions.hpp>
#include <Phox/Utilities/Conversions.hpp>
#include <iostream>

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
}

NetworkManager::NetworkManager()
{
    m_Listener.setBlocking(0);
    m_Socket.setBlocking(0);
}

void NetworkManager::doNetworkStuff()
{
    if (!m_Connected)//Listen to new connection
    {
        if (m_Hosting && m_Listener.accept(m_Socket) == sf::Socket::Done)
        {
            m_Connected = 1;
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
                    }
                    break;

                    case 1://Chat
                        m_Signal.clear();
                        m_Signal.writeString(m_OpponentName + ": " + Buffer.readString() + '\n');
                        break;

                    default: break;
                }
            }
        }

        if (status == sf::Socket::Disconnected)
        {
            m_Signal.clear();
            m_Signal << (m_OpponentName + " has disconnected.\n");
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
