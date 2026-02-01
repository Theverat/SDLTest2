#pragma once

#include <SDL3/SDL.h>

#include "Object.h"
#include "Player.h"
#include "Projectile.h"
#include "Utils.h"

#include <vector>

class Scene
{
public:
    Scene() {
        for (int i = 0; i < 10; ++i) {
            Object obj{};
            obj.setPos({ float(i * 50), float(i * 50), 0.0f });
            obj.setColor({ 1.0f, 0.0f, 0.0f, 1.f });
            enemies.push_back(obj);
        }

        player.setPos({ 200.0f, 100.0f, 0.0f });
        player.setColor({ 0.0f, 1.0f, 0.0f, 1.f });
    }

    ~Scene() {}

    void handleInput(const SDL_Event& event) {
        player.handleInput(event, *this);
    }

    void update(float dt) {
        using namespace DirectX;

        player.update(dt);

        for (auto& obj : enemies) {
            obj.update(dt);
        }

        for (auto& proj : projectiles) {
            proj.update(dt);

            // Check collision, for now naively
            for (auto& enemy : enemies) {
                const XMVECTOR toEnemy = enemy.getPos() - proj.getPos();
                const XMVECTOR midpointDistSq = XMVector3LengthSq(toEnemy);
                const XMVECTOR enemyRadius = toVec(enemy.getRadius());
                const XMVECTOR projRadius = toVec(proj.getRadius());

                const XMVECTOR dist = midpointDistSq - enemyRadius - projRadius;
                if (XMVector3Less(dist, XMVectorZero())) {
                    // Collision!

                    const int health = enemy.getHealth();
                    enemy.setHealth(health - proj.getDamage());
                    proj.setHealth(0);
                }
            }
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Object& obj) {
                return !obj.isAlive();
            }), enemies.end());

        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& proj) {
                return !proj.isAlive();
            }), projectiles.end());
    }

    void draw(SDL_Renderer* renderer) {
        player.draw(renderer);
        for (auto& obj : enemies) {
            obj.draw(renderer);
        }
        for (auto& proj : projectiles) {
            proj.draw(renderer);
        }
    }

    void addProjectile(const Projectile& projectile) {
        projectiles.push_back(projectile);
    }

private:
    std::vector<Object> enemies;
    std::vector<Projectile> projectiles;
    Player player{};
};

