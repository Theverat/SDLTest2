#include "Scene.h"

Scene::Scene()
{
    for (int i = 0; i < 10; ++i) {
        Enemy enemy{};
        enemy.setPos({ SDL_randf() * 2000.f, SDL_randf() * 2000.f, 0.0f });
        enemy.setColor({ 1.0f, 0.0f, 0.0f, 1.f });
        enemies.push_back(enemy);
    }

    player.setPos({ 200.0f, 100.0f, 0.0f });
    player.setColor({ 0.0f, 1.0f, 0.0f, 1.f });
}

void Scene::update(float dt, float elapsed)
{
    using namespace DirectX;

    // Updates

    player.update(dt, elapsed);
    player.maybeFire(*this, elapsed);

    for (auto& e : enemies) {
        e.doAIBehavior(*this, dt, elapsed);
        e.update(dt, elapsed);
    }

    for (auto& proj : projectiles) {
        proj.update(dt, elapsed);

        // Check collision, for now naively
        for (auto& enemy : enemies) {
            const XMVECTOR toEnemy = enemy.getPos() - proj.getPos();
            const XMVECTOR midpointDistSq = XMVector3LengthSq(toEnemy);
            const XMVECTOR enemyRadius = toVec(enemy.getRadius());
            const XMVECTOR projRadius = toVec(proj.getRadius());

            const XMVECTOR dist = midpointDistSq
                - enemyRadius * enemyRadius
                - projRadius * projRadius;

            if (XMVector3Less(dist, XMVectorZero())) {
                // Collision!
                const int health = enemy.getHealth();
                enemy.setHealth(health - proj.getDamage());
                proj.setHealth(0);
            }
        }
    }

    // Delete dead objects

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& enemy) {
            return !enemy.isAlive();
        }), enemies.end());

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& proj) {
            return !proj.isAlive();
        }), projectiles.end());
}
