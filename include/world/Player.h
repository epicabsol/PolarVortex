#pragma once

#include "Object.h"
#include "../render/SpriteInstance.h"

class InputDevice;
class DynamicCollider;
class SpriteAnimation;
class World;

enum class PlayerState {
    Idle,
    JumpCrouch,
    Midair,
    JumpRecover
};

class Player : public Object {
private:
    PlayerState _State;
    float _StateTime;
    InputDevice* _InputDevice;

    const SpriteAnimation* _IdleAnimation;
    const SpriteAnimation* _JumpAnimation;

    SpriteInstance _CurrentAnimation;
    DynamicCollider* _Collider;

    virtual void Added() override;
    virtual void Update(float timestep) override;
    virtual void Render() override;
    virtual void Removed() override;

public:
    Player(Allocator& allocator, InputDevice* inputDevice);

    inline InputDevice* GetInputDevice() const { return this->_InputDevice; }
    inline DynamicCollider* GetCollider() const { return this->_Collider; }
};