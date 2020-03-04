#pragma once
#include <ECSpp/EntityManager.h>

using STypeId_t = size_t;

class System {
public:
    virtual ~System() = default;
    virtual void update(epp::EntityManager& entityManager, float dt) = 0;

private:
    inline static STypeId_t typeCounter;

    template <class T>
    friend STypeId_t getSTypeId();
};


template <class T>
inline STypeId_t getSTypeId()
{
    static_assert(std::is_base_of_v<System, T>, "Only System based types can request Id here");
    static STypeId_t id = System::typeCounter++;
    return id;
}