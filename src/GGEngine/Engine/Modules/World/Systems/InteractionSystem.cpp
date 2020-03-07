#include <GGEngine/Engine/Modules/World/Components/DisplayComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/InteractionSystem.h>
#include <GGEngine/Game.h>

Vec2f crossProduct(const Vec2f& vec, float a);
Vec2f crossProduct(float a, const Vec2f& vec);

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
            phC.wheelAngVel[0] = std::clamp(phC.wheelAngVel[0] + 10.f, -5000.f, 5000.f);
        if (controllerModule.getActionKeyState(ActionKey::AccBackward) != KeyState::Released) {
            // phC.wheelAngVel[0] = std::clamp(phC.wheelAngVel[0] - 10.f, -5000.f, 5000.f);
            phC.wheelAngVel[0] = 0;
        }
        if (controllerModule.getActionKeyState(ActionKey::TurnRight) != KeyState::Released)
            phC.frontWheelsAngle = std::clamp(phC.frontWheelsAngle + toRadians(3.f), -toRadians(45.f), toRadians(45.f));
        else if (controllerModule.getActionKeyState(ActionKey::TurnLeft) != KeyState::Released)
            phC.frontWheelsAngle = std::clamp(phC.frontWheelsAngle - toRadians(3.f), -toRadians(45.f), toRadians(45.f));
        else
            phC.frontWheelsAngle = phC.frontWheelsAngle - sign(phC.frontWheelsAngle) * std::min(0.5f, std::abs(phC.frontWheelsAngle));
        float wheelsAngle = phC.frontWheelsAngle;

        Vec2f imps[2] = {};
        for (int i = 0; i < 2; ++i) {
            Vec2f const rToWheel = Vec2f(static_cast<PolygonShape const&>(shC.getShape()).getDetails().verts[i].x, 0) * asRotationMatrix(trC.getRotation());
            float const wheelR = 0.25f;
            Vec2f const ra(0.f, wheelR); // wheel's local
            Vec2f vRel = phC.velocity * asRotationMatrix(i == 1 ? -wheelsAngle : 0) * asRotationMatrix(-trC.getRotation()) +
                         crossProduct(phC.angularVelocity, rToWheel) * asRotationMatrix(i == 1 ? -wheelsAngle : 0) * asRotationMatrix(-trC.getRotation()) +
                         crossProduct(phC.wheelAngVel[i], ra);
            float vRelY = vRel.y;
            vRel.y = 0;
            float const wheelMOI = (phC.mass / 2.f * wheelR * wheelR) / 2.f;
            float const wheelMOIInv = 1.f / wheelMOI;
            float const wheelMassInv = phC.massInv * 2;
            float denominatorInv = 1.f / (wheelMassInv + dotProduct({ 1, 0 }, crossProduct(wheelMOIInv * crossProduct(ra, { 1, 0 }), ra)));
            Vec2f frictionImpulse = -vRel * denominatorInv;
            if (frictionImpulse.length() > 0.9f * 50.f * 9.81f * dt * denominatorInv) {
                std::cout << "Pale gume" << std::endl;
                frictionImpulse.normalize() *= 0.75f * 50.f * 9.81f * dt * denominatorInv;
            }
            phC.wheelAngVel[i] += wheelMOIInv * crossProduct(ra, frictionImpulse);
            imps[i] = frictionImpulse * asRotationMatrix(i == 1 ? wheelsAngle : 0) * asRotationMatrix(trC.getRotation());
            if (vRel.length() < 0.1f)
                phC.wheelAngVel[i] *= (1.f - phC.mass / 100000.f);

            vRel.x = 0;
            vRel.y = vRelY;
            vRel = vRel * asRotationMatrix(i == 1 ? wheelsAngle : 0) * asRotationMatrix(trC.getRotation());
            Vec2f tan = vRel.normalized();
            denominatorInv = 1.f / (phC.massInv + dotProduct(tan, crossProduct(phC.moiInv * crossProduct(rToWheel, tan), rToWheel)));
            frictionImpulse = -vRel * denominatorInv;
            float drifCoefficient = 10.f;
            if (frictionImpulse.length() > drifCoefficient * 9.81f * dt * denominatorInv) {
                std::cout << "Drift" << std::endl;
                frictionImpulse.normalize() *= 0.85f * drifCoefficient * 9.81f * dt * denominatorInv;
            }
            imps[i] += frictionImpulse;
        }
        for (int i = 0; i < 2; ++i) {
            phC.applyImpulse(imps[i], Vec2f(static_cast<PolygonShape const&>(shC.getShape()).getDetails().verts[i].x, 0) * asRotationMatrix(trC.getRotation()));
        }
    }
}