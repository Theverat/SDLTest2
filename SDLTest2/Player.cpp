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
                setSpeed(XMVectorSetX(getSpeed(), axisValue * MOVE_SPEED));
            }
            else
            {
                setSpeed(XMVectorSetX(getSpeed(), 0.0f));
            }
        }
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                setSpeed(XMVectorSetY(getSpeed(), axisValue * MOVE_SPEED));
            }
            else
            {
                setSpeed(XMVectorSetY(getSpeed(), 0.0f));
            }
        }
        // Right stick - Direction
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                setAimDir(XMVectorSetX(getAimDir(), axisValue));
            }
            else
            {
                setAimDir(XMVectorSetX(getAimDir(), 0.0f));
            }
        }
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
        {
            if (SDL_fabsf(axisValue) > STICK_DEADZONE)
            {
                setAimDir(XMVectorSetY(getAimDir(), axisValue));
            }
            else
            {
                setAimDir(XMVectorSetY(getAimDir(), 0.0f));
            }
        }
        // Right trigger - Fire
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)
        {
            firing = (axisValue > 0.5f);
        }
    }
}


void Player::maybeFire(Scene& scene, float elapsed)
{
    if (!firing || (elapsed - lastFireTime) < FIRE_INTERVAL) {
        return;
    }

    const XMVECTOR rawAimDir = getAimDir();
    if (XMVector3Less(XMVector3LengthSq(rawAimDir), XMVectorZero())) {
        return; // No aim direction
    }

    const XMVECTOR aimDir = XMVector3Normalize(rawAimDir);

    Projectile proj{};
    proj.setPos(getPos() + aimDir * 20.f);
    proj.setSpeed(aimDir * 1000.f);
    proj.setColor(toVec(1.0f, 1.0f, 0.0f, 1.0f));
    proj.setRadius(15.f);

    scene.addProjectile(proj);
    lastFireTime = elapsed;
}
