#include "Scene.h"

Scene::Scene()
{
    player.setPos({ bounds.w / 2.f, bounds.h / 2.f, 0.0f });
    player.setColor({ 0.0f, 1.0f, 0.0f, 1.f });
    spawnWave();
}

void Scene::spawnWave()
{
    const int enemyCount = 10;

    for (int i = 0; i < enemyCount; ++i) {
        Enemy enemy{};
        // Random point at the edge of the scene
        float x{}, y{};
        if (SDL_randf() > 0.5f) {
            x = SDL_randf() * bounds.w;
            y = SDL_randf() > 0.5f ? bounds.h : 0.f;
        }
        else {
            x = SDL_randf() > 0.5f ? bounds.w : 0.f;
            y = SDL_randf() * bounds.h;
        }
        enemy.setPos({ x, y, 0.0f });

        enemy.setColor({ SDL_randf() * 0.5f + 0.5f, 0.0f, 0.0f, 1.f });
        enemies.push_back(enemy);
    }
}

void Scene::update(float dt, float elapsed)
{
    using namespace DirectX;

    // Updates

    player.update(dt, elapsed);
    player.maybeFire(*this, elapsed);

    for (auto& enemy : enemies) {
        enemy.doAIBehavior(*this, dt, elapsed);
        enemy.update(dt, elapsed);

        if (checkCollision(enemy, player)) {
            enemy.maybeApplyDamage(player, elapsed);
        }
    }

    for (auto& proj : projectiles) {
        proj.update(dt, elapsed);

        // Check collision, for now naively
        for (auto& enemy : enemies) {
            if (checkCollision(enemy, proj)) {
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

void Scene::draw(SDL_Renderer* renderer, float elapsed,
    int width, int height, float uiScale)
{
    // Scale size of scene and everything in it to fit the window.
    const float windowAspect = float(width) / float(height);
    const float boundsAspect = bounds.w / bounds.h;

    SDL_FRect scaledBounds{};

    if (windowAspect > boundsAspect) {
        // Window is wider than bounds - letterbox with black bars on left/right
        scaledBounds.h = float(height);
        scaledBounds.w = scaledBounds.h * boundsAspect;
        scaledBounds.x = (float(width) - scaledBounds.w) * 0.5f;
        scaledBounds.y = 0.0f;
    }
    else {
        // Window is taller than bounds - pillarbox with black bars on top/bottom
        scaledBounds.w = float(width);
        scaledBounds.h = scaledBounds.w / boundsAspect;
        scaledBounds.x = 0.0f;
        scaledBounds.y = (float(height) - scaledBounds.h) * 0.5f;
    }

    // TODO do the same for other sprites/objects?

    // Draw ground
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &scaledBounds);

    player.draw(renderer);
    for (auto& obj : enemies) {
        obj.draw(renderer);
    }
    for (auto& proj : projectiles) {
        proj.draw(renderer);
    }

    // Player health bar
    SDL_FRect healthBar{
        scaledBounds.x + 20.f * uiScale,
        scaledBounds.y + 20.f * uiScale,
        player.getHealthPercent() * 200.f * uiScale,
        20.f * uiScale
    };
    SDL_Log("Health: %d, %.2f %", player.getHealth(), player.getHealthPercent());

    SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
    SDL_RenderFillRect(renderer, &healthBar);
}
