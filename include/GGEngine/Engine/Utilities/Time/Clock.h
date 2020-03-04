#pragma once
#include <SFML/System/Clock.hpp>
#include <cmath>
#include <stdint.h>

class SimulationClock {
    friend class Game;

public:
    using Clock_t = sf::Clock;

    using TimePoint_t = sf::Time;


public:
    static int32_t getTicksToMake();

    static float_t getDeltaTime();

    static float_t getWorldTime();

private:
    static void update();

public:
    static const float_t timestep;
};