#include "world/Player.h"

#include "assets/AssetManager.h"
#include "assets/Asset.h"
#include "game/PolarVortexGame.h"
#include "input/InputDevice.h"
#include "input/InputElement.h"
#include "render/SpriteAnimation.h"
#include "world/World.h"

#define FRAME_LENGTH (1.0f / 30.0f)
#define RUN_THRESHOLD (0.1f)

void Player::Added() {
    this->_Collider = this->_World->AddDynamicCollider(Vector2(0.0f, 3.0f), Vector2(0.15f, 0.45f), 1.0f);
}

void Player::Update(float timestep) {
    this->_CurrentAnimation.Advance(timestep);
    this->_StateTime += timestep;

    float movementSpeed = 2.5f;

    switch (this->_State) {
        case PlayerState::Idle: {
            if (this->_Collider->IsOnGround()) {
                if (this->_StateTime > FRAME_LENGTH * 2.0f && this->_InputDevice->GetInputElement(18)->GetValue() > 0.5f) {
                    this->EnterState(PlayerState::Crouch);
                }
                else if (this->_InputDevice->GetInputElement(0)->GetValue() > 0.5f) {
                    this->_Collider->GetVelocity().Y = 4.0;
                    this->EnterState(PlayerState::Midair);
                }
                else if (this->_Collider->GetVelocity().X > RUN_THRESHOLD || this->_Collider->GetVelocity().X < -RUN_THRESHOLD) {
                    this->EnterState(PlayerState::Run);
                }
            }
            else if (this->_StateTime > FRAME_LENGTH * 2.0f) {
                this->EnterState(PlayerState::Midair);
            }
            break;
        }
        case PlayerState::Crouch: {
            movementSpeed = 0.0f;
            if (this->_StateTime >= FRAME_LENGTH * 2.0f && this->_Collider->IsOnGround() && (this->_InputDevice->GetInputElement(0)->GetValue() > 0.5f)) {
                this->_Collider->GetVelocity().Y = 3.0f + HMM_MIN(this->_StateTime, 1.0f) * 3.0f;

                this->EnterState(PlayerState::Midair);
            }
            else if (this->_StateTime >= FRAME_LENGTH * 2.0f && !(this->_InputDevice->GetInputElement(18)->GetValue() > 0.5f)) {
                this->EnterState(PlayerState::CrouchStand);
            }
            break;
        }
        case PlayerState::Midair: {
            if (this->_Collider->IsOnGround()) {
                //this->EnterState(PlayerState::CrouchStand);
                this->EnterState(PlayerState::Idle);
            }
            else {
                if (this->_Collider->GetVelocity().Y > 1.0f) {
                    this->_CurrentAnimation.SetFrameIndex(3);
                }
                else if (this->_Collider->GetVelocity().Y < -1.0f) {
                    this->_CurrentAnimation.SetFrameIndex(5);
                }
                else {
                    this->_CurrentAnimation.SetFrameIndex(4);
                }
            }
            break;
        }
        case PlayerState::CrouchStand: {
            if (this->_StateTime >= FRAME_LENGTH) {
                this->EnterState(PlayerState::Idle);
            }
            break;
        }
        case PlayerState::Run: {
            if (this->_Collider->IsOnGround()) {
                if (this->_StateTime > FRAME_LENGTH * 2.0f && this->_InputDevice->GetInputElement(18)->GetValue() > 0.5f) {
                    // TODO: Slide animation
                    this->EnterState(PlayerState::Crouch);
                }
                else if (this->_InputDevice->GetInputElement(0)->GetValue() > 0.5f) {
                    this->_Collider->GetVelocity().Y = 4.0;
                    this->EnterState(PlayerState::Midair);
                }
                else if (this->_Collider->GetVelocity().X < RUN_THRESHOLD && this->_Collider->GetVelocity().X > -RUN_THRESHOLD) {
                    this->EnterState(PlayerState::Idle);
                }
                else {
                    this->_CurrentAnimation.SetTimeScale(HMM_ABS(this->_Collider->GetVelocity().X) / movementSpeed);
                }            }
            else if (this->_StateTime > FRAME_LENGTH * 2.0f) {
                this->EnterState(PlayerState::Midair);
            }

            break;
        }
    }

    float xvelocity = 0.0f;
    if (this->_InputDevice->GetInputElement(15)->GetValue() > 0.2f) {
        xvelocity -= this->_InputDevice->GetInputElement(15)->GetValue() * movementSpeed;
        this->_XFlip = -1.0f;
    }
    if (this->_InputDevice->GetInputElement(16)->GetValue() > 0.2f) {
        xvelocity += this->_InputDevice->GetInputElement(16)->GetValue() * movementSpeed;
        this->_XFlip = 1.0f;
    }
    this->_Collider->GetVelocity().X = xvelocity;
}

void Player::Render() {
    Game->GetRenderer().DrawSprite(this->_CurrentAnimation, this->_Collider->GetBounds().Position.X, this->_Collider->GetBounds().Position.Y, 0.0f, 1.0f * this->_XFlip, 1.0f);
}

void Player::Removed() {
    this->_World->RemoveDynamicCollider(this->_Collider);
}

void Player::EnterState(PlayerState state) {
    switch (state) {
        case PlayerState::Idle: {
            this->_CurrentAnimation.SetAnimation(this->_IdleAnimation);
            this->_CurrentAnimation.SetTimeScale(1.0f);
            this->_CurrentAnimation.SetLoop(true);
            break;
        }
        case PlayerState::Crouch: {
            this->_CurrentAnimation.SetAnimation(this->_CrouchAnimation);
            this->_CurrentAnimation.SetTimeScale(1.0f);
            this->_CurrentAnimation.SetLoop(false);
            break;
        }
        case PlayerState::Midair: {
            this->_CurrentAnimation.SetAnimation(this->_JumpAnimation);
            this->_CurrentAnimation.SetTimeScale(0.0f);
            this->_CurrentAnimation.SetFrameIndex(3);
            break;
        }
        case PlayerState::CrouchStand: {
            this->_CurrentAnimation.SetAnimation(this->_CrouchAnimation);
            this->_CurrentAnimation.SetTimeScale(0.0f);
            this->_CurrentAnimation.SetFrameIndex(0);
            break;
        }
        case PlayerState::Run: {
            this->_CurrentAnimation.SetAnimation(this->_RunAnimation);
            this->_CurrentAnimation.SetTimeScale(1.0f);
            this->_CurrentAnimation.SetFrameIndex(0);
            break;
        }
    }
    this->_State = state;
    this->_StateTime = 0.0f;
}

Player::Player(Allocator& allocator, InputDevice* inputDevice) : _State(PlayerState::Idle), _StateTime(0.0f), Object(allocator), _InputDevice(inputDevice), _XFlip(1.0f), _IdleAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_idle.pva"))->GetAsset<SpriteAnimation>()), _CrouchAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_crouch.pva"))->GetAsset<SpriteAnimation>()), _JumpAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_jump.pva"))->GetAsset<SpriteAnimation>()), _RunAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_run.pva"))->GetAsset<SpriteAnimation>()), _CurrentAnimation(allocator, this->_IdleAnimation), _Collider(nullptr) {

}