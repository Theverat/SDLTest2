#include "Player.h"

#include "Projectile.h"
#include "Scene.h"

using namespace DirectX;

void Player::handleInput(const SDL_Event& event, Scene& scene)
{
    const float STICK_DEADZONE = 0.15f;
    const float MOVE_SPEED = 200.0f;
    const XMVECTOR MIN_DIR_LEN_SQ = toVec(STICK_DEADZONE * STICK_DEADZONE);

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
            const XMVECTOR dir = XMVectorSetX(getAimDir(), axisValue);
            if (XMVector2Greater(XMVector2LengthSq(dir), MIN_DIR_LEN_SQ))
                setAimDir(dir);
            else
                setAimDir(XMVectorZero());
        }
        else if (event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
        {
            const XMVECTOR dir = XMVectorSetY(getAimDir(), axisValue);
            if (XMVector2Greater(XMVector2LengthSq(dir), MIN_DIR_LEN_SQ))
                setAimDir(dir);
            else
                setAimDir(XMVectorZero());
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
    static const XMVECTOR minLen = XMVectorReplicate(0.1f);
    if (XMVector3Less(XMVector3LengthSq(rawAimDir), minLen)) {
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
