#include <Chess/NetworkManager.hpp>
#include <Phox/Utilities/Conversions.hpp>
#include <iostream>

NetworkManager::NetworkManager()
{
    m_Listener.setBlocking(0);
    m_Socket.setBlocking(0);
}

void NetworkManager::doNetworkStuff()
{
    if (m_Hosting)
    {
        if (!m_Connected)//Listen to new connection
        {
            if (m_Listener.accept(m_Socket) == sf::Socket::Done)
            {
                std::cout << "Nected!\n";
                m_Connected = 1;
            }
        }

        else

        {
            char Data[512];
            std::size_t Size;
            sf::Socket::Status status = m_Socket.receive(Data, 512, Size);

            std::cout << "Size: " << Size << '\n';

            if (Size)
            {
                if (status == sf::Socket::Done)
                {
                    std::cout << "Got data\n";
                }

                if (status == sf::Socket::Disconnected)
                {
                    std::cout << "Disconnect\n";
                }

                if (status == sf::Socket::Error)
                {
                    std::cout << "Error\n";
                }

                if (status == sf::Socket::NotReady)
                {
                    std::cout << "Not Ready\n";
                }
            }
        }
    }

    else

    {
        if (m_Connected)
        {
            Phox::cStreamBuffer buff;
            buff.writeUnsignedByte(0);
            buff.writeString("Hey");
            m_Socket.send(buff.getConstPointer(), buff.getWorkingBytes());
            std::cout << "Sent\n";
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
            std::cout << "Connected\n";
        }
    }

    else if (Sig == "Host")
    {
        unsigned short Port = static_cast<unsigned short> (Phox::ToDouble(Signal.readString()));
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
}

Phox::cStreamBuffer NetworkManager::getGUISignal()
{
    auto Return = m_Signal;
    m_Signal.clear();
    return Return;
}
