#include "Enemy.h"

#include "Scene.h"

using namespace DirectX;

void Enemy::doAIBehavior(const Scene& scene, float dt, float elapsed)
{
    const Player& player = scene.getPlayer();
    const XMVECTOR toPlayer = player.getPos() - getPos();
    const XMVECTOR dirToPlayer = XMVector3Normalize(toPlayer);

    // Simple chase behavior
    XMVECTOR accel = dirToPlayer * 50.f;
    XMVECTOR newSpeed = getSpeed() + accel * dt;
    // Clamp speed
    newSpeed = XMVectorMin(XMVectorMax(newSpeed, -MAX_SPEED), MAX_SPEED);
    setSpeed(newSpeed);
}
