#include <Chess/Includes.hpp>

std::string stringReplaceAll(const std::string& str, const std::string& subStr, const std::string& rep)
{
    std::string Ret = str;
    auto pos = Ret.find(subStr);

    while (pos != std::string::npos)
    {
        Ret.erase(Ret.begin() + pos, Ret.begin() + pos + subStr.size());
        Ret.insert(pos, rep);
        pos = Ret.find(subStr);
    }

    return Ret;
}

int main(int argc, char* argv[])
{
    std::cout << stringReplaceAll("He55llo!", "5", "_lol_");
    sf::RenderWindow Window(sf::VideoMode(1072, 600), "Chess");
    Phox::Mouse mouse;
    Phox::Keyboard keyboard;
    Game daGame;//John's board
    GUIManager GUI;//GUI handling
    Board GameBoard;//Ruck's GUI board
    NetworkManager Network;

    //Set stuff up
    {
        Window.setFramerateLimit(30);
        mouse.setReferenceWindow(Window);
        daGame = newGame();//John's board

        GUI.init(Window);

        if (!GameBoard.loadResources())
        {
            return -1;//If I couldn't load the textures or find files, exit.
        }
        GameBoard.linkGUIManager(GUI);
    }

    while (Window.isOpen())
    {
        Phox::clearIODevices(mouse, keyboard);//Clear Mouse/Keyboard states

        sf::Event Event;
        bool closeWindow = 0;
        while (Window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)              closeWindow = 1;
            if (Event.type == sf::Event::KeyPressed)          keyboard.tellPressed(Event.key.code);
            if (Event.type == sf::Event::KeyReleased)         keyboard.tellReleased(Event.key.code);
            if (Event.type == sf::Event::MouseButtonPressed)  mouse.tellPressed(Event.mouseButton.button);
            if (Event.type == sf::Event::MouseButtonReleased) mouse.tellReleased(Event.mouseButton.button);
            GUI.update(Event);//Pass the event to the GUI manager
        }

        mouse.update();//Update the Mouse positions

        if (closeWindow)
        {
            Window.close();
            break;
        }


        Window.clear(sf::Color(110, 110, 110));//Clear the window with a green color

        bool myMove = !Network.isConnected();
        if (!myMove)//Connected
        {
            if (Network.isBoardFresh())
            {
                std::memcpy(daGame, Network.getRefreshedBoard(), sizeof(game));
            }
            myMove = (Network.isHosting() && Network.isWhitesMove()) || (!Network.isHosting() && !Network.isWhitesMove());
        }

        GameBoard.networkDraw(Network.isConnected(), myMove, Network.getMyName(), Network.getOpponentName(), Network.getTimeServer(), Network.getTimeOpponent());
        GameBoard.update(daGame, mouse.getPosition(Window), Window);//Send John's board state to Ruck's board
        GameBoard.getOpponentMove(daGame, Network.reportMove());

        if (Network.requestCurrentBoard())
        {
            Network.setCurrentBoard(daGame);
        }

        if (mouse.pressed(sf::Mouse::Button::Left) && myMove)//Forward a mouse click to Ruck's board
        {
            GameBoard.clicked(daGame, mouse.getPosition(Window));
            Network.getMove(GameBoard.getNetworkMove());
        }

        Network.handleSignal(GUI.getNetworkSignal());//Inform the network manager of GUI events
        GUI.handleSignal(Network.getGUISignal());

        std::size_t New = GUI.getNewGame();
        if (New)
        {
            if (Network.isConnected() && !Network.isHosting())
            {
                Phox::cStreamBuffer Buff;
                Buff.writeString("Cannot start a new game while in a network game.\n");
                GUI.handleSignal(Buff);
            }
            else
            {
                deleteGame(daGame);
                daGame = newGame();
                GameBoard.reset();
                if (New == 2)
                {
                    randomize960(daGame);
                }
                if (Network.isConnected())
                {
                    GUI.saveGame(Network.getOpponentName(), Network.getTimeServer(), Network.getTimeOpponent());
                    if (Network.isHosting())
                    {
                        Network.refreshBoard(daGame);
                    }
                }
                else
                GUI.saveGame();
            }
        }
        GUI.draw();

        if (GameBoard.getParsing())
        {
            sf::sleep(sf::milliseconds(GameBoard.getParsingWaitTime()));
        }

        Window.display();//Draw everything on the window
    }

    if (Network.isConnected())
    {
        GUI.saveGame(Network.getOpponentName(), Network.getTimeServer(), Network.getTimeOpponent());
    }

    else

    {
        GUI.saveGame();
    }
    deleteGame(daGame);

    return 1;
}
