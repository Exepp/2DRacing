#include <GGEngine/Engine/Modules/Controller/ControllerModule.h>
#include <GGEngine/Engine/Utilities/Math/Transform.h>
#include <GGEngine/Game.h>

ControllerModule::ControllerModule()
{
    setBinding({ sf::Mouse::Button::Left, InputDivice::Mouse }, ActionKey::LMB);
    setBinding({ sf::Mouse::Button::Right, InputDivice::Mouse }, ActionKey::RMB);

    setBinding({ sf::Keyboard::Key::W, InputDivice::Keyboard }, ActionKey::MoveUp);
    setBinding({ sf::Keyboard::Key::S, InputDivice::Keyboard }, ActionKey::MoveDown);
    setBinding({ sf::Keyboard::Key::A, InputDivice::Keyboard }, ActionKey::MoveLeft);
    setBinding({ sf::Keyboard::Key::D, InputDivice::Keyboard }, ActionKey::MoveRight);

    setBinding({ sf::Keyboard::Key::Up, InputDivice::Keyboard }, ActionKey::AccForward);
    setBinding({ sf::Keyboard::Key::Down, InputDivice::Keyboard }, ActionKey::AccBackward);
    setBinding({ sf::Keyboard::Key::Left, InputDivice::Keyboard }, ActionKey::TurnLeft);
    setBinding({ sf::Keyboard::Key::Right, InputDivice::Keyboard }, ActionKey::TurnRight);

    setBinding({ sf::Keyboard::Key::LControl, InputDivice::Keyboard }, ActionKey::ZoomIn);
    setBinding({ sf::Keyboard::Key::Space, InputDivice::Keyboard }, ActionKey::ZoomOut);

    setBinding({ sf::Keyboard::Key::Num1, InputDivice::Keyboard }, ActionKey::Num1);
    setBinding({ sf::Keyboard::Key::Num2, InputDivice::Keyboard }, ActionKey::Num2);
    setBinding({ sf::Keyboard::Key::Num3, InputDivice::Keyboard }, ActionKey::Num3);
    setBinding({ sf::Keyboard::Key::Num4, InputDivice::Keyboard }, ActionKey::Num4);
    setBinding({ sf::Keyboard::Key::Num5, InputDivice::Keyboard }, ActionKey::Num5);

    setBinding({ sf::Keyboard::Key::Escape, InputDivice::Keyboard }, ActionKey::Options);
};

void ControllerModule::update()
{
    response();
    updateControllerState();
    updateCursorState();
}

void ControllerModule::setBinding(const RawKey& raw, ActionKey action)
{
    bindings[raw] = action;
    states[action] = KeyState::Released;
}

void ControllerModule::updateCursorState()
{
    cursorWorldPos = Game::Modules.input.getCurrentInput().signedCursorPos +
                     asVec<Vec2f>(Game::Modules.window.getWin().getView().getCenter());
    cursorWorldPos /= Settings<WindowModule>::WorldToViewRatio;
    cursorWorldDeltaPos = Game::Modules.input.getCurrentInput().cursorDeltaPos;
    cursorWorldDeltaPos /= Settings<WindowModule>::WorldToViewRatio;
}

void ControllerModule::updateControllerState()
{
    const RawInputPack& rawInput = Game::Modules.input.getCurrentInput();

    // set as HeldDown every keyState, that in previous tick was "Pressed"
    for (auto& stateNode : states)
        if (stateNode.second == KeyState::Pressed)
            stateNode.second = KeyState::HeldDown;

    for (const auto& key : rawInput.rawKeys)
        if (auto found = bindings.find(key); found != bindings.end())
            states[found->second] = key.state;
}

void ControllerModule::response()
{
    {
        sf::View view = Game::Modules.window.getWin().getView();
        float ratio = Game::Modules.window.getViewToWindowRatio().length();

        if (getActionKeyState(ActionKey::MoveUp) != KeyState::Released)
            view.setCenter(view.getCenter() + sf::Vector2f(0.f, -0.1f) * Settings<WindowModule>::WorldToViewRatio * ratio);
        if (getActionKeyState(ActionKey::MoveDown) != KeyState::Released)
            view.setCenter(view.getCenter() + sf::Vector2f(0, 0.1f) * Settings<WindowModule>::WorldToViewRatio * ratio);
        if (getActionKeyState(ActionKey::MoveLeft) != KeyState::Released)
            view.setCenter(view.getCenter() + sf::Vector2f(-0.1f, 0.f) * Settings<WindowModule>::WorldToViewRatio * ratio);
        if (getActionKeyState(ActionKey::MoveRight) != KeyState::Released)
            view.setCenter(view.getCenter() + sf::Vector2f(0.1f, 0.f) * Settings<WindowModule>::WorldToViewRatio * ratio);

        if (getActionKeyState(ActionKey::ZoomIn) != KeyState::Released)
            view.setSize(view.getSize() / 1.01f);
        if (getActionKeyState(ActionKey::ZoomOut) != KeyState::Released)
            view.setSize(view.getSize() * 1.01f);

        Game::Modules.window.getWin().setView(view);
    }
}

const ControllerModule::RawToActionMap& ControllerModule::getBindings() const
{
    return bindings;
}

KeyState ControllerModule::getActionKeyState(ActionKey actionKey) const
{
    return states.at(actionKey);
}

const Vec2f& ControllerModule::getCursorWorldPosition() const
{
    return cursorWorldPos;
}

const Vec2f& ControllerModule::getCursorWorldDeltaPosition() const
{
    return cursorWorldDeltaPos;
}