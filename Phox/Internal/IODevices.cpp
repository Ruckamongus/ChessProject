#include <Phox/Internal/IODevices.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
namespace Phox
{
    namespace Internal
    {
        extern sf::RenderWindow* Window;
    }
}

void Phox::clearIODevices()
{
    for (unsigned int i = 0; i < 100; i++)
    {
        Keyboard.m_catchPress[i] = 0;
        Keyboard.m_catchRelease[i] = 0;
    }

    for (unsigned int i = 0; i < 3; i++)
    {
        Mouse.m_catchPress[i] = 0;
        Mouse.m_catchRelease[i] = 0;
    }
}

void Phox::cMouse::update()
{
    x = sf::Mouse::getPosition(*Phox::Internal::Window).x;
    y = sf::Mouse::getPosition(*Phox::Internal::Window).y;
    x_abs = sf::Mouse::getPosition().x;
    y_abs = sf::Mouse::getPosition().y;
}

bool Phox::cKeyboard::anyKeyPressed()
{
    for (unsigned int i = 0; i < 100; i++)
    {
        if (pressed(static_cast<sf::Keyboard::Key> (i)))
            return 1;
    }
    return 0;
}

bool Phox::cKeyboard::anyKeyReleased()
{
    for (unsigned int i = 0; i < 100; i++)
    {
        if (released(static_cast<sf::Keyboard::Key> (i)))
            return 1;
    }
    return 0;
}

bool Phox::cKeyboard::anyKeyDown()
{
    for (unsigned int i = 0; i < 100; i++)
    {
        if (down(static_cast<sf::Keyboard::Key> (i)))
            return 1;
    }
    return 0;
}
