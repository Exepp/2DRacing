#pragma once
#include <GGEngine/Engine/Modules/Input/InputModule.h>
#include <array>
#include <unordered_map>

struct ControllerComponent;

enum class ActionKey : uint8_t {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    ZoomIn,
    ZoomOut,
    AccForward,
    AccBackward,
    TurnLeft,
    TurnRight,
    LMB,
    RMB,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Options,
    Count
};

class ControllerModule {

    using RawToActionMap = std::unordered_map<RawKey, ActionKey>;

    using ActionStateMap = std::unordered_map<ActionKey, KeyState>;

public:
    ControllerModule();

    void update();

    const RawToActionMap& getBindings() const;

    KeyState getActionKeyState(ActionKey actionKey) const;

    const Vec2f& getCursorWorldPosition() const;

    const Vec2f& getCursorWorldDeltaPosition() const;

private:
    void setBinding(const RawKey& raw, ActionKey action);

    void updateCursorState();

    void updateControllerState();

    void response();

private:
    RawToActionMap bindings;

    ActionStateMap states;

    Vec2f cursorWorldPos;

    Vec2f cursorWorldDeltaPos;
};