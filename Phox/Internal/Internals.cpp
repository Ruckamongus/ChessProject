#include <Phox/Internal/Internals.hpp>
#include <sstream>

namespace
{
    const std::string GetOpenGLContext(const sf::RenderWindow& Window)
    {
        std::stringstream Return;
        sf::ContextSettings OpenGLVersion = Window.getSettings();
        Return << OpenGLVersion.majorVersion << "." << OpenGLVersion.minorVersion;
        return Return.str();
    }

    bool CloseWindow = 0;
    bool MainWindowActive = 1;
}


namespace Phox
{
    namespace Internal
    {
        sf::Clock FPSClock;
        float FPSCurrentTime = 0.f;
        float FPS = 0.f;
        sf::RenderWindow* Window = nullptr;
    }

    bool PhoxEngineInit()
    {
        if (Internal::Window == nullptr)
            Internal::Window = new sf::RenderWindow(sf::VideoMode(800, 600, 32), "Chess");
        return 1;
    }

    bool PhoxEngineFree()
    {
        if (Internal::Window != nullptr)
            delete Internal::Window;
        return 0;
    }

    const std::string EngineVersion() {return "1.4.0.4";}

    const std::string OpenGLVersion()
    {
        if (Internal::Window == nullptr)
            return "Undefined. Call PhoxEngineInit() first.";
        return GetOpenGLContext(*Internal::Window);
    }

    cMouse Mouse;
    cKeyboard Keyboard;

    #ifdef PHOX_DEBUG_MODE_ENABLED
        cDebugLog Debugger;
    #endif
}

namespace Phox
{
    bool PhoxWindowClosing() {return CloseWindow;}
    bool PhoxWindowActive()  {return MainWindowActive;}

    void PhoxMainLoop(bool skipEvents)
    {
        if (!skipEvents)
        {
            clearIODevices();
            sf::Event Event;
            while (Internal::Window->pollEvent(Event))
            {
                if (Event.type == sf::Event::Closed)      CloseWindow = 1;
                if (Event.type == sf::Event::KeyPressed)  Keyboard.tellPressed(Event.key.code);
                if (Event.type == sf::Event::KeyReleased) Keyboard.tellReleased(Event.key.code);
                if (Event.type == sf::Event::MouseButtonPressed)  Mouse.tellPressed(Event.mouseButton.button);
                if (Event.type == sf::Event::MouseButtonReleased) Mouse.tellReleased(Event.mouseButton.button);
                if (Event.type == sf::Event::LostFocus)   {MainWindowActive = 0;}
                if (Event.type == sf::Event::GainedFocus) {MainWindowActive = 1;}
            }
            Mouse.update();
        }

        static float Time = 1.f;
        Time = Internal::FPSClock.restart().asSeconds();
        Internal::FPS = 1.f / Internal::FPSCurrentTime;
        Internal::FPSCurrentTime = Time;
    }

    float PhoxGetFPS()
    {
        return Internal::FPS;
    }
}
