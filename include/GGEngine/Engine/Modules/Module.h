#pragma once
#include <ECSpp/utility/Assert.h>
#include <ECSpp/utility/TuplePP.h>
#include <GGEngine/Engine/Engine.h>
#include <memory>


template <class... DTypes> // DTypes = Dependencies (other modules)
class Module {
public:
    virtual void update() = 0;
};