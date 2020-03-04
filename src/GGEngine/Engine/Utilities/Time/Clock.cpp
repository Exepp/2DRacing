#include <GGEngine/Engine/Utilities/Math/Transform.h>
#include <GGEngine/Engine/Utilities/Time/Clock.h>
#include <SFML/System/Sleep.hpp>
#include <iostream>

const float_t SimulationClock::timestep = 1.f / 60.f;

static SimulationClock::Clock_t clk;
static SimulationClock::TimePoint_t last;

static float_t deltaTime = SimulationClock::timestep;

static float_t timeOffset = 0.f;

static int32_t ticksToMake = 0;

void SimulationClock::update()
{
    if (last == TimePoint_t::Zero)
        last = clk.getElapsedTime();


    TimePoint_t sleepDuration = sf::seconds(timestep - (clk.getElapsedTime() - last).asSeconds() - timeOffset);
    //if(sleepDuration.asMilliseconds())
    sf::sleep(sleepDuration);


    TimePoint_t now = clk.getElapsedTime();

    deltaTime = (now - last).asSeconds();
    last = now;

    timeOffset += deltaTime - timestep;

    ticksToMake = int32_t(timeOffset / timestep);
    timeOffset -= ticksToMake * timestep;
}

int32_t SimulationClock::getTicksToMake()
{
    return ticksToMake + 1;
}

float_t SimulationClock::getWorldTime()
{
    return clk.getElapsedTime().asSeconds();
}


float_t SimulationClock::getDeltaTime()
{
    return deltaTime;
}
