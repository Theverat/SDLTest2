#include "Player.h"

#include "Projectile.h"
#include "Scene.h"

using namespace DirectX;

void Player::handleInput(const SDL_Event& event, Scene& scene)
{
    const float STICK_DEADZONE = 0.15f;
    const float MOVE_SPEED = 200.0f;

    // Handle gamepad axis motion (joysticks)
    if (event.type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
    {
        float axisValue = event.gaxis.value / 32767.0f; // Normalize to -1.0 to 1.0

        // Left stick - Movement
        if (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                DirectX::XMFLOAT3 speed = getSpeed();
                speed.x = axisValue * MOVE_SPEED;
                setSpeed(speed);
            }
            else
            {
                DirectX::XMFLOAT3 speed = getSpeed();
                speed.x = 0.0f;
                setSpeed(speed);
            }
        }
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                DirectX::XMFLOAT3 speed = getSpeed();
                speed.y = axisValue * MOVE_SPEED;
                setSpeed(speed);
            }
            else
            {
                DirectX::XMFLOAT3 speed = getSpeed();
                speed.y = 0.0f;
                setSpeed(speed);
            }
        }
        // Right stick - Direction
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                DirectX::XMFLOAT3 dir = getAimDir();
                dir.x = axisValue;
                setAimDir(dir);
            }
            else
            {
                DirectX::XMFLOAT3 dir = getAimDir();
                dir.x = 0.0f;
                setAimDir(dir);
            }
        }
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                DirectX::XMFLOAT3 dir = getAimDir();
                dir.y = axisValue;
                setAimDir(dir);
            }
            else
            {
                DirectX::XMFLOAT3 dir = getAimDir();
                dir.y = 0.0f;
                setAimDir(dir);
            }
        }
    }
    // Handle gamepad button presses
    else if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
    {
        // X button (Cross on PlayStation) - Fire
        if (event.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH)
        {
            fire(scene);
        }
    }
}

void Player::fire(Scene& scene)
{
    XMVECTOR pos = getPosV();

    Projectile proj{};
    proj.setPos(getPos());
}
