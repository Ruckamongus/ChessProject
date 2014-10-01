#include <Chess\Includes.hpp>
#include <Chess\GUIManager.hpp>
#include <Chess\John\JohnDefinitions.hpp>
#include <TGUI\TGUI.hpp>

int main(int argc, char* argv[])
{
    sf::RenderWindow Window(sf::VideoMode(800, 600), "Chess");
    Phox::Mouse mouse;
    Phox::Keyboard keyboard;
    Game daGame;//John's board
    GUIManager GUI;//GUI handling
    Board GameBoard;//Ruck's GUI board

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

        GameBoard.update(daGame, mouse.getPosition(Window), Window);//Send John's board state to Ruck's board

        if (mouse.pressed(sf::Mouse::Button::Left))//Forward a mouse click to Ruck's board
        {
            GameBoard.clicked(daGame, mouse.getPosition(Window));
        }

        std::size_t New = GUI.getNewGame();
        if (New)
        {
            deleteGame(daGame);
            daGame = newGame();
            GameBoard.reset();
            if (New == 2)
            {
                randomize960(daGame);
            }
        }
        GUI.draw();

        if (GameBoard.getParsing())
        {
            sf::sleep(sf::milliseconds(GameBoard.getParsingWaitTime()));
        }

        Window.display();//Draw everything on the window
    }

    deleteGame(daGame);

    return 1;
}
