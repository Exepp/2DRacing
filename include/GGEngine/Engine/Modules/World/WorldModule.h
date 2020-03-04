#pragma once
#include <ECSpp/EntityManager.h>
#include <GGEngine/Engine/Modules/World/Systems/DisplaySystem.h>
#include <GGEngine/Engine/Modules/World/Systems/InteractionSystem.h>
#include <GGEngine/Engine/Modules/World/Systems/PhysicsSystem.h>

class InteractionSystem;
class PhysicsSystem;
class DisplaySystem;

class WorldModule {
    template <class T>
    using UPtr = std::unique_ptr<T>;

public:
    WorldModule();

    void update();

private:
    epp::EntityManager mgr;
    UPtr<InteractionSystem> interactionSystem;
    UPtr<PhysicsSystem> physicsSystem;
    UPtr<DisplaySystem> displaySystem;
};