#pragma once
#include <ECSpp/EntityManager.h>
#include <GGEngine/Engine/Modules/World/Components/ShapeComponent.h>
#include <GGEngine/Engine/Modules/World/Components/TransformComponent.h>
#include <GGEngine/Engine/Utilities/Math/Vec2.h>
#include <array>
#include <deque>
#include <unordered_map>

class Grid {
public:
    constexpr static std::size_t CellSize = 32;

private:
    struct BigEnt {
        epp::Entity ent;
        AABBi mapBounds;
    };

    struct Cell_t {
        using Ents_t = std::vector<epp::Entity>;
        using EntsBounds_t = std::vector<AABBi>;

        Ents_t ents;
        EntsBounds_t bounds;
    };

    using CellsDeq_t = std::deque<Cell_t>;
    using CellsMap_t = std::unordered_map<Vec2i, Cell_t*>;
    using QueryResult_t = std::vector<Cell_t const*>;

public:
    void insert(epp::Entity ent, Vec2f const& pos, const AABBf& aabb);
    void swapWithBigEnts();
    void clear();

    CellsDeq_t const& getCells() const { return cellsDeq; }
    CellsMap_t const& getMap() const { return cellsMap; }
    Cell_t const& getBigEnts() const { return bigEnts; }

    static AABBi CalcMapBounds(Vec2f const& pos, AABBf const& aabb)
    {
        static auto const CalcIdx = [](Vec2f::Val_t cord) { return cord != 0.f ? ptrdiff_t(std::ceil(abs(cord) / CellSize)) * sign(cord) : 1; };
        return { Vec2i(CalcIdx(pos.x + aabb.min.x), CalcIdx(pos.y + aabb.min.y)),
                 Vec2i(CalcIdx(pos.x + aabb.max.x), CalcIdx(pos.y + aabb.max.y)) };
    }

private:
    CellsDeq_t cellsDeq;
    CellsMap_t cellsMap;
    Cell_t bigEnts;
    QueryResult_t queryResult;

    friend class CollisionSystem;
};