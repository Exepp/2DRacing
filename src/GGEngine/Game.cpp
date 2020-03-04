#include <GGEngine/Engine/Utilities/Time/Clock.h>
#include <GGEngine/Game.h>

Game::ModulesStruct Game::Modules;

void Game::Run()
{
    while (Modules.window.getWin().isOpen()) {
        SimulationClock::update();
        Modules.window.update();
        Modules.input.update();
        Modules.controller.update();
        Modules.world.update();
        Modules.display.update();
    }
}