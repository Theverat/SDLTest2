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

    void spawnWave();

    void update(float dt, float elapsed);
    void draw(SDL_Renderer* renderer, float elapsed,
        int width, int height, float uiScale);

    void addProjectile(const Projectile& projectile) {
        projectiles.push_back(projectile);
    }

private:
    SDL_FRect bounds{ 0, 0, 1920, 1080 };
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    Player player{};
};
