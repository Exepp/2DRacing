#pragma once
#include <GGEngine/Engine/Modules/Controller/ControllerModule.h>
#include <GGEngine/Engine/Modules/Display/DisplayModule.h>
#include <GGEngine/Engine/Modules/Input/InputModule.h>
#include <GGEngine/Engine/Modules/Window/WindowModule.h>
#include <GGEngine/Engine/Modules/World/WorldModule.h>

class Game {

    // order of adding modules determines the order of modules startup, update and
    // (reversed) shutdown!
    struct ModulesStruct {
        WindowModule window;
        InputModule input;
        ControllerModule controller;
        WorldModule world;
        DisplayModule display;
    };

private:
    Game() = delete;

public:
    static void Run();

    static ModulesStruct Modules;
};