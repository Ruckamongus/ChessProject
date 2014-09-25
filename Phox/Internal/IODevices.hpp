#ifndef __PhoxEngineIODevices__
#define __PhoxEngineIODevices__ 1
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace Phox
{
    void clearIODevices();

    class cMouse
    {
        public:
            #ifdef PHOX_ENABLE_INLINES
                inline bool pressed(sf::Mouse::Button Button) {return m_catchPress[Button];}
                inline bool released(sf::Mouse::Button Button) const {return m_catchRelease[Button];}
                inline bool down(sf::Mouse::Button Button) const {return  sf::Mouse::isButtonPressed(Button);}
                inline bool up  (sf::Mouse::Button Button) const {return !sf::Mouse::isButtonPressed(Button);}
                inline void setPosition(const int X, const int Y) const {sf::Mouse::setPosition(sf::Vector2i(X, Y));}
                inline void setPosition(const int X, const int Y, const sf::Window& Target) const {sf::Mouse::setPosition(sf::Vector2i(X, Y), Target);}
                inline int x_rel_to(const sf::Window& Target) const {return sf::Mouse::getPosition(Target).x;}
                inline int y_rel_to(const sf::Window& Target) const {return sf::Mouse::getPosition(Target).y;}
                inline sf::Vector2i getPosition() const {return sf::Mouse::getPosition();}
                inline sf::Vector2f getPositionFloat() const {return sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);}
                inline sf::Vector2i getPosition(const sf::Window& Target) const {return sf::Mouse::getPosition(Target);}
                inline sf::Vector2f getPositionFloat(const sf::Window& Target) const {return sf::Vector2f(sf::Mouse::getPosition(Target).x, sf::Mouse::getPosition(Target).y);}
            #else
                bool pressed(sf::Mouse::Button Button) {return m_catchPress[Button];}
                bool released(sf::Mouse::Button Button) const {return m_catchRelease[Button];}
                bool down(sf::Mouse::Button Button) const {return  sf::Mouse::isButtonPressed(Button);}
                bool up  (sf::Mouse::Button Button) const {return !sf::Mouse::isButtonPressed(Button);}
                void setPosition(const int X, const int Y) const {sf::Mouse::setPosition(sf::Vector2i(X, Y));}
                void setPosition(const int X, const int Y, const sf::Window& Target) const {sf::Mouse::setPosition(sf::Vector2i(X, Y), Target);}
                int x_rel_to(const sf::Window& Target) const {return sf::Mouse::getPosition(Target).x;}
                int y_rel_to(const sf::Window& Target) const {return sf::Mouse::getPosition(Target).y;}
                sf::Vector2i getPosition() const {return sf::Mouse::getPosition();}
                sf::Vector2f getPositionFloat() const {return sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);}
                sf::Vector2i getPosition(const sf::Window& Target) const {return sf::Mouse::getPosition(Target);}
                sf::Vector2f getPositionFloat(const sf::Window& Target) const {return sf::Vector2f(sf::Mouse::getPosition(Target).x, sf::Mouse::getPosition(Target).y);}
            #endif

            void update();

            int x;
            int y;
            int x_abs;
            int y_abs;

            //friend void clearIODevices();
            //friend void PhoxMainLoop();

        //private:
            #ifdef PHOX_ENABLE_INLINES
                inline void tellPressed(sf::Mouse::Button Button)  {m_catchPress[Button] = 1; m_catchRelease[Button] = 0;}
                inline void tellReleased(sf::Mouse::Button Button) {m_catchRelease[Button] = 1; m_catchPress[Button] = 0;}
            #else
                void tellPressed(sf::Mouse::Button Button)  {m_catchPress[Button] = 1; m_catchRelease[Button] = 0;}
                void tellReleased(sf::Mouse::Button Button) {m_catchRelease[Button] = 1; m_catchPress[Button] = 0;}
            #endif
            bool m_catchPress[3] = {0};
            bool m_catchRelease[3]= {0};
    };

    class cKeyboard
    {
        public:
            #ifdef PHOX_ENABLE_INLINES
                explicit inline cKeyboard() {clearIODevices();}
                inline bool down(sf::Keyboard::Key Key) const {return  sf::Keyboard::isKeyPressed(Key);}
                inline bool up  (sf::Keyboard::Key Key) const {return !sf::Keyboard::isKeyPressed(Key);}
                inline bool pressed (sf::Keyboard::Key Key) const {return m_catchPress[Key];}
                inline bool released(sf::Keyboard::Key Key) const {return m_catchRelease[Key];}
            #else
                explicit cKeyboard() {clearIODevices();}
                bool down(sf::Keyboard::Key Key) const {return  sf::Keyboard::isKeyPressed(Key);}
                bool up  (sf::Keyboard::Key Key) const {return !sf::Keyboard::isKeyPressed(Key);}
                bool pressed (sf::Keyboard::Key Key) const {return m_catchPress[Key];}
                bool released(sf::Keyboard::Key Key) const {return m_catchRelease[Key];}
            #endif
            bool anyKeyPressed();
            bool anyKeyReleased();
            bool anyKeyDown();

            //friend void clearIODevices();
            //friend void PhoxMainLoop();

        //private:
            bool m_catchPress[100] = {0};
            bool m_catchRelease[100] = {0};
            #ifdef PHOX_ENABLE_INLINES
                inline void tellPressed (sf::Keyboard::Key Key) {m_catchPress[Key] = 1; m_catchRelease[Key] = 0;}
                inline void tellReleased(sf::Keyboard::Key Key) {m_catchRelease[Key] = 1; m_catchPress[Key] = 0;}
            #else
                void tellPressed (sf::Keyboard::Key Key) {m_catchPress[Key] = 1; m_catchRelease[Key] = 0;}
                void tellReleased(sf::Keyboard::Key Key) {m_catchRelease[Key] = 1; m_catchPress[Key] = 0;}
            #endif
    };

    extern cMouse Mouse;
    extern cKeyboard Keyboard;
}
#endif
