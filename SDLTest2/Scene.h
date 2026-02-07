#pragma once

#include <SDL3/SDL.h>

#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Utils.h"

#include <vector>
#include <span>

class Scene
{
public:
    Scene();
    ~Scene() {}

    const Player& getPlayer() const { return player; }
    std::span<const Enemy> getEnemies() const { return enemies; }
    std::span<const Projectile> getProjectiles() const { return projectiles; }

    void handleInput(const SDL_Event& event) {
        player.handleInput(event, *this);
    }

    void spawnEnemiesOnSceneEdge(int enemyCount);
    bool isGameOver() const {
        return player.getHealth() <= 0;
    }
    void restart();
    void update(float dt, float elapsed);
    void draw(SDL_Renderer* renderer, float elapsed,
        int width, int height, float uiScale);

    void addProjectile(const Projectile& projectile) {
        projectiles.push_back(projectile);
    }

    void togglePause() {
        SDL_Log("Toggling pause");
        paused = !paused;
    }

private:
    SDL_FRect bounds{ 0, 0, 1920, 1080 };
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    Player player{};
    bool playerTookDamage{ false };
    bool paused{ false };

    struct Wave {
        int counter{ 0 };
        int enemyCount{ 20 };
        int smartEnemyCount{ 1 };
        float lastSpawnTime{};
        float spawnInterval{ 20.f };
    } wave;

    struct Stats {
        size_t enemiesKilled{ 0 };
    } stats;
};
