#include <Chess\Includes.hpp>
#include <Chess\GUIManager.hpp>
#include <Chess\John\JohnDefinitions.hpp>
#include <TGUI\TGUI.hpp>

int main(int argc, char* argv[])
{
    using namespace Phox;
    PhoxEngineInit();//Init the internal stuff like window, FPS handler, mouse, and keyboard
    sf::RenderWindow& Window = *Internal::Window;//Make a shortcut refernce to the internal window
    Window.setFramerateLimit(30);//room_speed = 30


    Game daGame = newGame();//John's board

    std::cout << "Odd: " << int(0%2);


    GUIManager GUI;
    if (!GUI.init(Window)) return -2;

    Board GameBoard;//Rucks GUI board
    if (!GameBoard.loadResources()) return -1;//If I couldn't load the textures or find files, exit.
    GameBoard.linkGUIManager(GUI);

    while (Window.isOpen())
    {
        clearIODevices();
        sf::Event Event;
        bool closeWindow = 0;
        while (Window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)      closeWindow = 1;
            if (Event.type == sf::Event::KeyPressed)  Keyboard.tellPressed(Event.key.code);
            if (Event.type == sf::Event::KeyReleased) Keyboard.tellReleased(Event.key.code);
            if (Event.type == sf::Event::MouseButtonPressed)  Mouse.tellPressed(Event.mouseButton.button);
            if (Event.type == sf::Event::MouseButtonReleased) Mouse.tellReleased(Event.mouseButton.button);
            GUI.update(Event);
        }
        Mouse.update();

        PhoxMainLoop(!false);//Handle FPS
        if (closeWindow) {Window.close(); break;}


        Window.clear(sf::Color(110, 110, 110));//Clear the window with a green color

        GameBoard.update(daGame);//Send John's board state to Ruck's board

        if (Mouse.pressed(sf::Mouse::Button::Left))//Forward a mouse click to Ruck's board
            GameBoard.clicked(daGame);

        GUI.doCallbacks();
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

        Window.display();//Draw everything on the window
        Window.setTitle("  FPS: " + toString(PhoxGetFPS()));//Draw FPS in the window's title
    }

    deleteGame(daGame);

    PhoxEngineFree();//Free the internal game window
    return 1;
}
