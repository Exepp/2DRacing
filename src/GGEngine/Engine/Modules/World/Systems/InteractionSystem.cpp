#include <GGEngine/Engine/Modules/World/Components/DisplayComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/InteractionSystem.h>
#include <GGEngine/Game.h>

void InteractionSystem::update(epp::EntityManager& entityManager, float dt)
{
    const ControllerModule& controllerModule = Game::Modules.controller;
    if (controllerModule.getActionKeyState(ActionKey::Options) == KeyState::Pressed)
        Game::Modules.window.getWin().close();

    entityManager.updateSelection(playableEnt);
    EPP_ASSERTA_M(playableEnt.countEntities(), "Only one playable entity is permited");
    if (auto ent = playableEnt.begin(); ent != playableEnt.end()) {
        TransformComponent& trC = ent.getComponent<TransformComponent>();
        PhysicsComponent& phC = ent.getComponent<PhysicsComponent>();
        ShapeComponent& shC = ent.getComponent<ShapeComponent>();
        if (controllerModule.getActionKeyState(ActionKey::AccForward) != KeyState::Released)
            trC.moveForward(0.1f);
        if (controllerModule.getActionKeyState(ActionKey::AccBackward) != KeyState::Released)
            trC.moveForward(-0.1f);

        if (controllerModule.getActionKeyState(ActionKey::AccForward) != KeyState::Released ||
            controllerModule.getActionKeyState(ActionKey::AccBackward) != KeyState::Released) {
            if (controllerModule.getActionKeyState(ActionKey::TurnRight) != KeyState::Released)
                trC.rotate(0.1f);
            if (controllerModule.getActionKeyState(ActionKey::TurnLeft) != KeyState::Released)
                trC.rotate(-0.1f);
        }
    }
}