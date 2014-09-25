#ifndef __PhoxEngineIODevices__
#define __PhoxEngineIODevices__ 1
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

namespace sf
{
    class RenderWindow;
}

namespace Phox
{
    class Keyboard;

    class Mouse
    {
        public:
            Mouse() = default;
           ~Mouse() = default;

            Mouse(const Mouse& other)=delete;
            Mouse(const Mouse&& other)=delete;
            Mouse& operator=(Mouse other)=delete;
            Mouse& operator=(Mouse&& other)=delete;

            inline bool pressed(sf::Mouse::Button Button)
            {
                return m_catchPress[Button];
            }

            inline bool released(sf::Mouse::Button Button) const
            {
                return m_catchRelease[Button];
            }

            inline bool down(sf::Mouse::Button Button) const
            {
                return  sf::Mouse::isButtonPressed(Button);
            }

            inline bool up  (sf::Mouse::Button Button) const
            {
                return !sf::Mouse::isButtonPressed(Button);
            }

            inline void setPosition(const int X, const int Y) const
            {
                sf::Mouse::setPosition(sf::Vector2i(X, Y));
            }

            inline void setPosition(const int X, const int Y, const sf::Window& Target) const
            {
                sf::Mouse::setPosition(sf::Vector2i(X, Y), Target);
            }

            inline int x_rel_to(const sf::Window& Target) const
            {
                return sf::Mouse::getPosition(Target).x;
            }

            inline int y_rel_to(const sf::Window& Target) const
            {
                return sf::Mouse::getPosition(Target).y;
            }

            inline sf::Vector2i getPosition() const
            {
                return sf::Mouse::getPosition();
            }

            inline sf::Vector2f getPositionFloat() const
            {
                return sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
            }

            inline sf::Vector2i getPosition(const sf::Window& Target) const
            {
                return sf::Mouse::getPosition(Target);
            }

            inline sf::Vector2f getPositionFloat(const sf::Window& Target) const
            {
                return sf::Vector2f(sf::Mouse::getPosition(Target).x, sf::Mouse::getPosition(Target).y);
            }

            inline int x() const {return m_x;}
            inline int y() const {return m_y;}
            inline int xAbs() const {return m_xAbs;}
            inline int yAbs() const {return m_yAbs;}

            void tellPressed(sf::Mouse::Button Button);
            void tellReleased(sf::Mouse::Button Button);
            void update();
            void setReferenceWindow(sf::RenderWindow& window);
            friend void clearIODevices(Mouse& mouse, Keyboard& keyboard);

        private:


            int m_x;
            int m_y;
            int m_xAbs;
            int m_yAbs;

            bool m_catchPress[3] = {0};
            bool m_catchRelease[3]= {0};

            sf::RenderWindow* m_ReferenceWindow = nullptr;
    };

    class Keyboard
    {
        public:
            Keyboard()=default;
           ~Keyboard()=default;
            Keyboard(const Keyboard& other)=delete;
            Keyboard(const Keyboard&& other)=delete;
            Keyboard& operator=(Keyboard other)=delete;
            Keyboard& operator=(Keyboard&& other)=delete;

            inline bool down(sf::Keyboard::Key Key) const
            {
                return  sf::Keyboard::isKeyPressed(Key);
            }

            inline bool up(sf::Keyboard::Key Key) const
            {
                return !sf::Keyboard::isKeyPressed(Key);
            }

            inline bool pressed(sf::Keyboard::Key Key) const
            {
                return m_catchPress[Key];
            }

            inline bool released(sf::Keyboard::Key Key) const
            {
                return m_catchRelease[Key];
            }

            bool anyKeyPressed();
            bool anyKeyReleased();
            bool anyKeyDown();
            void tellPressed (sf::Keyboard::Key Key);
            void tellReleased(sf::Keyboard::Key Key);

            friend void clearIODevices(Mouse& mouse, Keyboard& keyboard);

        private:
            bool m_catchPress[100] = {0};
            bool m_catchRelease[100] = {0};
    };

    void clearIODevices(Mouse& mouse, Keyboard& keyboard);

}
#endif
