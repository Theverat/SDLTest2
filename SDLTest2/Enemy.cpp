#include "Enemy.h"

#include "Scene.h"
#include "Utils.h"

using namespace DirectX;

void Enemy::doAIBehavior(const Scene& scene, float dt, float elapsed)
{
    if (aiMode == AIMode::DUMB_CHASE) {
        doDumbChase(scene, dt, elapsed);
    }
    else if (aiMode == AIMode::PREDICTIVE_CHASE) {
        doPredictiveChase(scene, dt, elapsed);
    }
}

void Enemy::doDumbChase(const Scene& scene, float dt, float elapsed)
{
    const Player& player = scene.getPlayer();
    const XMVECTOR toPlayer = player.getPos() - getPos();
    const XMVECTOR dirToPlayer = XMVector3Normalize(toPlayer);

    const XMVECTOR accel = dirToPlayer * acceleration;
    const XMVECTOR newSpeed = getSpeed() + accel * dt;
    const XMVECTOR MAX_SPEED = toVec(maxSpeed);
    setSpeed(clamp(newSpeed, -MAX_SPEED, MAX_SPEED));
}

void Enemy::doPredictiveChase(const Scene& scene, float dt, float elapsed)
{
    const Player& player = scene.getPlayer();
    const XMVECTOR futurePos = player.getPos() + player.getSpeed();

    const XMVECTOR toFuturePos = futurePos - getPos();
    const XMVECTOR dirToFuturePos = XMVector3Normalize(toFuturePos);

    // Also take into account our own speed
    const XMVECTOR futureEnemyPos = getPos() + getSpeed();
    const XMVECTOR toFutureEnemyPos = futureEnemyPos - futurePos;
    const XMVECTOR dirToFutureEnemyPos = XMVector3Normalize(toFutureEnemyPos);
    const float dot = XMVectorGetX(XMVector2Dot(dirToFutureEnemyPos, dirToFuturePos));
    // Adjust this for more or less prediction
    const float predictionFactor = 0.5f;
    const XMVECTOR predictedFuturePos = futurePos
        + player.getSpeed() * predictionFactor * dot;

    const XMVECTOR toPredictedFuturePos = predictedFuturePos - getPos();
    const XMVECTOR dirToFuturePos2 = XMVector3Normalize(toPredictedFuturePos);

    const XMVECTOR accel = dirToFuturePos2 * acceleration;
    const XMVECTOR newSpeed = getSpeed() + accel * dt;
    const XMVECTOR MAX_SPEED = toVec(maxSpeed);
    setSpeed(clamp(newSpeed, -MAX_SPEED, MAX_SPEED));
}
