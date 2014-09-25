#include <Phox/Internal/IODevices.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Phox
{
    void clearIODevices(Mouse& mouse, Keyboard& keyboard)
    {
        for (std::size_t i = 0; i < 100; ++i)
        {
            keyboard.m_catchPress[i] = 0;
            keyboard.m_catchRelease[i] = 0;
        }

        for (std::size_t i = 0; i < 3; ++i)
        {
            mouse.m_catchPress[i] = 0;
            mouse.m_catchRelease[i] = 0;
        }
    }

    void Mouse::setReferenceWindow(sf::RenderWindow& window)
    {
        m_ReferenceWindow = &window;
    }

    void Mouse::update()
    {
        if (m_ReferenceWindow != nullptr)
        {
            m_x = sf::Mouse::getPosition(*m_ReferenceWindow).x;
            m_y = sf::Mouse::getPosition(*m_ReferenceWindow).y;
        }
        m_xAbs = sf::Mouse::getPosition().x;
        m_yAbs = sf::Mouse::getPosition().y;
    }

    void Mouse::tellPressed(sf::Mouse::Button Button)
    {
        m_catchPress[Button] = 1;
        m_catchRelease[Button] = 0;
    }
    void Mouse::tellReleased(sf::Mouse::Button Button)
    {
        m_catchRelease[Button] = 1;
        m_catchPress[Button] = 0;
    }

    bool Keyboard::anyKeyPressed()
    {
        for (std::size_t i = 0; i < 100; ++i)
        {
            if (pressed(static_cast<sf::Keyboard::Key> (i)))
            {
                return 1;
            }
        }
        return 0;
    }

    bool Keyboard::anyKeyReleased()
    {
        for (std::size_t i = 0; i < 100; ++i)
        {
            if (released(static_cast<sf::Keyboard::Key> (i)))
            {
                return 1;
            }
        }
        return 0;
    }

    bool Keyboard::anyKeyDown()
    {
        for (std::size_t i = 0; i < 100; ++i)
        {
            if (down(static_cast<sf::Keyboard::Key> (i)))
            {
                return 1;
            }
        }
        return 0;
    }

    void Keyboard::tellPressed (sf::Keyboard::Key Key)
    {
        m_catchPress[Key] = 1;
        m_catchRelease[Key] = 0;
    }

    void Keyboard::tellReleased(sf::Keyboard::Key Key)
    {
        Keyboard::m_catchRelease[Key] = 1;
        m_catchPress[Key] = 0;
    }
}
