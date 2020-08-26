#include "world/Player.h"

#include "assets/AssetManager.h"
#include "assets/Asset.h"
#include "game/PolarVortexGame.h"
#include "input/InputDevice.h"
#include "input/InputElement.h"
#include "render/SpriteAnimation.h"
#include "world/World.h"

void Player::Added() {
    this->_Collider = this->_World->AddDynamicCollider(Vector2(0.0f, 3.0f), Vector2(0.15f, 0.35f), 1.0f);
}

void Player::Update(float timestep) {
    this->_CurrentAnimation.Advance(timestep);
    this->_StateTime += timestep;

    float movementSpeed = 3.0f;

    switch (this->_State) {
        case PlayerState::Idle: {
            if (this->_Collider->IsOnGround() && this->_InputDevice->GetInputElement(0)->GetValue() > 0.5f) {
                //this->_Collider->GetVelocity().Y = 5.0f;
                this->_State = PlayerState::JumpCrouch;
                this->_CurrentAnimation.SetAnimation(this->_JumpAnimation);
                this->_CurrentAnimation.SetTimeScale(0.0f);
                this->_CurrentAnimation.SetFrameIndex(1);
                this->_StateTime = 0.0f;
            }
            break;
        }
        case PlayerState::JumpCrouch: {
            if (this->_Collider->IsOnGround() && !(this->_InputDevice->GetInputElement(0)->GetValue() > 0.5f)) {
                this->_Collider->GetVelocity().Y = 3.0f + HMM_MIN(this->_StateTime, 1.0f) * 3.0f;

                this->_State = PlayerState::Midair;
                //this->_CurrentAnimation.SetAnimation(this->_JumpAnimation);
                //this->_CurrentAnimation.SetTimeScale(0.0f);
                this->_CurrentAnimation.SetFrameIndex(2);
                this->_StateTime = 0.0f;
            }
            break;
        }
        case PlayerState::Midair: {
            if (this->_Collider->IsOnGround()) {
                this->_State = PlayerState::JumpRecover;
                //this->_CurrentAnimation.SetAnimation(this->_JumpAnimation);
                //this->_CurrentAnimation.SetTimeScale(0.0f);
                this->_CurrentAnimation.SetFrameIndex(5);
                this->_StateTime = 0.0f;
            }
            else {
                if (this->_Collider->GetVelocity().Y > 1.0f) {
                    this->_CurrentAnimation.SetFrameIndex(2);
                }
                else if (this->_Collider->GetVelocity().Y < -1.0f) {
                    this->_CurrentAnimation.SetFrameIndex(4);
                }
                else {
                    this->_CurrentAnimation.SetFrameIndex(3);
                }
            }
            break;
        }
        case PlayerState::JumpRecover: {
            if (this->_StateTime >= 0.25f) {
                this->_State = PlayerState::Idle;
                this->_CurrentAnimation.SetAnimation(this->_IdleAnimation);
                this->_CurrentAnimation.SetTimeScale(1.0f);
            }
            break;
        }
    }

    float xvelocity = 0.0f;
    if (this->_InputDevice->GetInputElement(15)->GetValue() > 0.2f) {
        xvelocity -= this->_InputDevice->GetInputElement(15)->GetValue() * movementSpeed;
    }
    if (this->_InputDevice->GetInputElement(16)->GetValue() > 0.2f) {
        xvelocity += this->_InputDevice->GetInputElement(16)->GetValue() * movementSpeed;
    }
    this->_Collider->GetVelocity().X = xvelocity;
}

void Player::Render() {
    Game->GetRenderer().DrawSprite(this->_CurrentAnimation, this->_Collider->GetBounds().Position.X, this->_Collider->GetBounds().Position.Y, 0.0f, 1.0f, 1.0f);
}

void Player::Removed() {
    this->_World->RemoveDynamicCollider(this->_Collider);
}

Player::Player(Allocator& allocator, InputDevice* inputDevice) : _State(PlayerState::Idle), _StateTime(0.0f), Object(allocator), _InputDevice(inputDevice), _IdleAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_idle.pva"))->GetAsset<SpriteAnimation>()), _JumpAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_jump.pva"))->GetAsset<SpriteAnimation>()), _CurrentAnimation(allocator, this->_IdleAnimation), _Collider(nullptr) {

}