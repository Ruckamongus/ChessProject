#ifndef __PhoxInternalsHPP__
#define __PhoxInternalsHPP__ 1
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <Phox/Internal/IODevices.hpp>
#include <Phox/Internal/Exception.hpp>

#include <string>

namespace Phox
{
    namespace Internal
    {
        extern sf::Clock FPSClock;
        extern float FPSCurrentTime;
        extern float FPS;
        extern sf::RenderWindow* Window;
    }

    bool PhoxWindowClosing();
    void PhoxMainLoop(bool skipEvents = 0);
    bool PhoxWindowActive();
    bool PhoxEngineInit();
    bool PhoxEngineFree();
    float PhoxGetFPS();
    const std::string EngineVersion();
    const std::string OpenGLVersion();
}
#endif
