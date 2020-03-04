#pragma once
#include <GGEngine/Engine/Modules/World/Components/ShapeComponent.h>
#include <GGEngine/Engine/Modules/World/Components/TransformComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/CollisionSystem/Grid.h>
#include <GGEngine/Engine/Modules/World/Systems/System.h>
#include <thread>

struct MyManifold {
    MyManifold() = default;
    MyManifold(c2Manifold c2) : count(c2.count), depths{ c2.depths[0], c2.depths[1] }, contact_points{ asVec<Vec2f>(c2.contact_points[0]), asVec<Vec2f>(c2.contact_points[1]) }, n(asVec<Vec2f>(c2.n))
    {}

    size_t count = 0;
    float depths[2];
    Vec2f contact_points[2];
    Vec2f n;
};

struct StaticObjectTag {};


class CollisionSystem {
public:
    template <typename... OtherCTypes, typename Cb_t>
    void run(epp::EntityManager& mgr,
             epp::Selection<TransformComponent, ShapeComponent, OtherCTypes...>& dynamicEnts,
             epp::Selection<TransformComponent, ShapeComponent, OtherCTypes..., StaticObjectTag>& staticEnts,
             Cb_t callback)
    {
        EPP_ASSERT(dynamicEnts.getUnwanted().contains(epp::IdOfL<StaticObjectTag>()));

        grid.clear();
        dynamicEnts.forEach([&](auto const& it) {
            ShapeComponent& shC = it.template getComponent<ShapeComponent>();
            TransformComponent& trC = it.template getComponent<TransformComponent>();
            shC.getShape().setTransform(trC.getTransform());
            grid.insert(*it, trC.getPosition(), shC.getShape().getAABB());
        });

        // (dynamic - dynamic) & (small - small)
        auto fn = [this, &dynamicEnts, &mgr, &callback](std::size_t start, std::size_t end) {
            for (; start < end; ++start)
                if (grid.cellsDeq[start].ents.size() > 1)
                    for (std::size_t i = 0; i < grid.cellsDeq[start].ents.size(); ++i) {
                        auto first = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(grid.cellsDeq[start].ents[i])); // only entities of dynamicEnts were inserted above,
                                                                                                                   // so this iterator will always point to the correct entity (never beyond)
                        for (auto j = i + 1; j < grid.cellsDeq[start].ents.size(); ++j) {
                            auto second = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(grid.cellsDeq[start].ents[j]));
                            c2Manifold manifold = first.template getComponent<ShapeComponent>().getShape().collisionManifold(
                                second.template getComponent<ShapeComponent>().getShape());
                            if (manifold.count)
                                callback(manifold, first, second);
                        }
                    }
        };
        std::thread t1(fn, 0, grid.cellsDeq.size() / 4);
        std::thread t2(fn, grid.cellsDeq.size() / 4, grid.cellsDeq.size() / 2);
        std::thread t3(fn, grid.cellsDeq.size() / 2, 3 * grid.cellsDeq.size() / 4);
        fn(3 * grid.cellsDeq.size() / 4, grid.cellsDeq.size());
        if (t1.joinable())
            t1.join();
        if (t2.joinable())
            t2.join();
        if (t3.joinable())
            t3.join();
        // static - (dynamic & (small - small))
        staticEnts.forEach([&](auto const& it) {
            ShapeComponent& shC = it.template getComponent<ShapeComponent>();
            TransformComponent& trC = it.template getComponent<TransformComponent>();
            shC.getShape().setTransform(trC.getTransform());

            AABBi bounds = Grid::CalcMapBounds(trC.getPosition(), shC.getShape().getAABB());
            for (ptrdiff_t x = bounds.min.x; x <= bounds.max.x; ++x)
                for (ptrdiff_t y = bounds.min.y; x != 0 && y <= bounds.max.y; ++y)
                    if (y != 0)
                        if (auto found = grid.cellsMap.find(Vec2i(x, y)); found != grid.cellsMap.end())
                            for (std::size_t i = 0; i < found->second->ents.size(); ++i) {
                                AABBi commonBounds = bounds & found->second->bounds[i];
                                if (Vec2i(x, y) != commonBounds.min)
                                    continue;

                                auto dynEntIt = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(found->second->ents[i]));
                                c2Manifold manifold = dynEntIt.template getComponent<ShapeComponent>().getShape().collisionManifold(shC.getShape());
                                if (manifold.count)
                                    callback(manifold, dynEntIt, it);
                            }
        });

        // (dynamic - dynamic) & (big - small)
        for (std::size_t i = 0; i < grid.bigEnts.ents.size(); ++i) {
            auto bigEntIt = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(grid.bigEnts.ents[i]));
            for (ptrdiff_t x = grid.bigEnts.bounds[i].min.x; x <= grid.bigEnts.bounds[i].max.x; ++x)
                for (ptrdiff_t y = grid.bigEnts.bounds[i].min.y; x != 0 && y <= grid.bigEnts.bounds[i].max.y; ++y)
                    if (y != 0)
                        if (auto found = grid.cellsMap.find(Vec2i(x, y)); found != grid.cellsMap.end())
                            for (std::size_t j = 0; j < found->second->ents.size(); ++j) {
                                AABBi commonBounds = grid.bigEnts.bounds[i] & found->second->bounds[j];
                                if (Vec2i(x, y) != commonBounds.min)
                                    continue;

                                auto smallEntIt = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(found->second->ents[j]));
                                c2Manifold manifold = smallEntIt.template getComponent<ShapeComponent>().getShape().collisionManifold(
                                    bigEntIt.template getComponent<ShapeComponent>().getShape());
                                if (manifold.count)
                                    callback(manifold, smallEntIt, bigEntIt);
                            }
        }

        // (dynamic - dynamic) & (big - big)
        grid.swapWithBigEnts(); // small ones removed, big ones inserted
        for (auto const& gridCell : grid.cellsMap)
            if (gridCell.second->ents.size() > 1)
                for (std::size_t i = 0; i < gridCell.second->ents.size(); ++i) {
                    auto first = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(gridCell.second->ents[i])); // only entities of dynamicEnts were inserted above,
                                                                                                           // so this iterator will always point to the correct entity (never beyond)
                    for (auto j = i + 1; j < gridCell.second->ents.size(); ++j) {
                        AABBi commonBounds = gridCell.second->bounds[i] & gridCell.second->bounds[j];
                        if (gridCell.first != commonBounds.min)
                            continue;
                        auto second = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(gridCell.second->ents[j]));
                        c2Manifold manifold = first.template getComponent<ShapeComponent>().getShape().collisionManifold(
                            second.template getComponent<ShapeComponent>().getShape());
                        if (manifold.count)
                            callback(manifold, first, second);
                    }
                }

        // static - (dynamic & big)
        staticEnts.forEach([&](auto const& it) {
            ShapeComponent& shC = it.template getComponent<ShapeComponent>();
            TransformComponent& trC = it.template getComponent<TransformComponent>();
            shC.getShape().setTransform(trC.getTransform());

            AABBi bounds = Grid::CalcMapBounds(trC.getPosition(), shC.getShape().getAABB());
            for (ptrdiff_t x = bounds.min.x; x <= bounds.max.x; ++x)
                for (ptrdiff_t y = bounds.min.y; x != 0 && y <= bounds.max.y; ++y)
                    if (y != 0)
                        if (auto found = grid.cellsMap.find(Vec2i(x, y)); found != grid.cellsMap.end())
                            for (std::size_t i = 0; i < found->second->ents.size(); ++i) {
                                AABBi commonBounds = bounds & found->second->bounds[i];
                                if (Vec2i(x, y) != commonBounds.min)
                                    continue;

                                auto dynEntIt = dynamicEnts.begin().jumpToOrBeyond(mgr.cellOf(found->second->ents[i]));
                                c2Manifold manifold = dynEntIt.template getComponent<ShapeComponent>().getShape().collisionManifold(shC.getShape());
                                if (manifold.count)
                                    callback(manifold, dynEntIt, it);
                            }
        });
    }

    const Grid& getGrid() const { return grid; }

private:
    void updateGrid(epp::EntityManager& entMgr);
    void findCollisions(epp::EntityManager& entMgr);

private:
    Grid grid;
};