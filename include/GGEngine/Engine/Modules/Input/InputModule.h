#pragma once
#include <GGEngine/Engine/Modules/Input/RawKey.h>
#include <GGEngine/Engine/Modules/Window/WindowModule.h>
#include <array>

struct RawInputPack {
    void resetKeys() { rawKeys = std::array<RawKey, maxInput>(); }

    static const uint8_t maxInput = 6;

    std::array<RawKey, maxInput> rawKeys;

    Vec2f absCursorPos;    // position of a cursor on a window <0; winSize>
    Vec2f signedCursorPos; // vector from window's center to cursor <-winSize / 2;
                           // winSize / 2>
    Vec2f cursorDeltaPos;
};

class InputModule {
public:
    void update();

    const RawInputPack& getCurrentInput() const;

private:
    void assignMousePosition();
    void assignInputKeys();

private:
    RawInputPack currentInput;
};
