#include <GGEngine/Engine/Modules/World/Systems/PhysicsSystem.h>


static Vec2f crossProduct(const Vec2f& vec, float a)
{
    return Vec2f(a * vec.y, -a * vec.x);
}


static Vec2f crossProduct(float a, const Vec2f& vec)
{
    return -crossProduct(vec, a);
}

void PhysicsSystem::update(epp::EntityManager& entityManager, float dt)
{
    entityManager.updateSelection(dynamicEnts);
    entityManager.updateSelection(staticEnts);

    resolveCollisions(entityManager);
    applyGravity(dt);
    resolveVelocities(dt);
}

void PhysicsSystem::resolveCollisions(epp::EntityManager& entMgr)
{
    cs.run(entMgr, dynamicEnts, staticEnts,
           [](MyManifold const& manifold, DynamicSelIterator_t const& first, auto const& second) {
               auto& transformC = first.getComponent<TransformComponent>();
               auto& physicsC = first.getComponent<PhysicsComponent>();


               auto& othertransformC = second.template getComponent<TransformComponent>();
               auto& otherPhysicsC = second.template getComponent<PhysicsComponent>();

               float e = minValue(physicsC.restitution, otherPhysicsC.restitution);
               //    for (int abc = 0; abc < 5; ++abc)
               for (size_t i = 0; i < manifold.count; ++i) {
                   Vec2f ra = manifold.contact_points[i] - transformC.getPosition();
                   Vec2f rb = manifold.contact_points[i] - othertransformC.getPosition();

                   Vec2f rVel = otherPhysicsC.velocity - physicsC.velocity + crossProduct(otherPhysicsC.angularVelocity, rb) - crossProduct(physicsC.angularVelocity, ra) - manifold.depths[i] * manifold.n * 10.f;
                   float rVelMagnitudeAlongN = dotProduct(rVel, manifold.n);

                   if (rVelMagnitudeAlongN > 0)
                       continue;

                   float raCrossN = crossProduct(ra, manifold.n);
                   float rbCrossN = crossProduct(rb, manifold.n);
                   float invMassSum = physicsC.massInv + otherPhysicsC.massInv + raCrossN * raCrossN * physicsC.moiInv + rbCrossN * rbCrossN * otherPhysicsC.moiInv;

                   float j = -(1.f + e) * rVelMagnitudeAlongN;
                   j /= invMassSum;
                   j /= manifold.count;

                   Vec2f impulse = j * manifold.n;
                   physicsC.applyImpulse(-impulse, ra);
                   otherPhysicsC.applyImpulse(impulse, rb);

                   rVel = otherPhysicsC.velocity - physicsC.velocity + crossProduct(otherPhysicsC.angularVelocity, rb) - crossProduct(physicsC.angularVelocity, ra);

                   // Solve for the tangent vector
                   Vec2f tangent = rVel - dotProduct(rVel, manifold.n) * manifold.n;

                   tangent.normalize();

                   // Solve for magnitude to apply along the friction vector
                   float jt = -dotProduct(rVel, tangent);
                   jt /= invMassSum;
                   jt /= manifold.count;

                   if (abs(jt) <= 0.001f)
                       continue;

                   float mu = (physicsC.staticFriction + otherPhysicsC.staticFriction) * 0.5f;

                   // Clamp magnitude of friction and create impulse vector
                   Vec2f frictionImpulse;
                   float dynamicFriction = 0.f;
                   if (abs(jt) < j * mu)
                       frictionImpulse = jt * tangent;
                   else {
                       dynamicFriction = (physicsC.dynamicFriction + otherPhysicsC.dynamicFriction) * 0.5f;
                       frictionImpulse = -j * tangent * dynamicFriction;
                   }

                   physicsC.applyImpulse(-frictionImpulse, ra);
                   otherPhysicsC.applyImpulse(frictionImpulse, rb);
               }
           });
}

void PhysicsSystem::applyGravity(float dt)
{
    for (auto it = dynamicEnts.begin(); it != dynamicEnts.end(); ++it)
        it.getComponent<PhysicsComponent>().velocity += Vec2f(0.f, 19.81f) * dt;
}

void PhysicsSystem::resolveVelocities(float dt)
{
    for (auto it = dynamicEnts.begin(); it != dynamicEnts.end(); ++it) {
        it.getComponent<TransformComponent>().moveGlobal(it.getComponent<PhysicsComponent>().velocity * dt);
        it.getComponent<TransformComponent>().rotate(it.getComponent<PhysicsComponent>().angularVelocity * dt);
    }
    for (auto it = staticEnts.begin(); it != staticEnts.end(); ++it) {
        it.getComponent<TransformComponent>().moveGlobal(it.getComponent<PhysicsComponent>().velocity * dt);
        it.getComponent<TransformComponent>().rotate(it.getComponent<PhysicsComponent>().angularVelocity * dt);
    }
}