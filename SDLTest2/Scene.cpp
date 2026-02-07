#include "Scene.h"

using namespace DirectX;

Scene::Scene()
{
    player.setPos({ bounds.w / 2.f, bounds.h / 2.f, 0.0f });
    player.setColor({ 0.0f, 1.0f, 0.0f, 1.f });
}

void Scene::spawnEnemiesOnSceneEdge(int enemyCount)
{
    assert(enemyCount > 0);
    std::vector<XMFLOAT2> spawnPoints;
    for (int i = 0; i < enemyCount; ++i) {
        Enemy enemy{};
        // Random point at the edge of the scene
        XMFLOAT2 point{};
        bool tooClose = false;
        do {
            if (SDL_randf() > 0.5f) {
                point.x = SDL_randf() * bounds.w;
                point.y = SDL_randf() > 0.5f ? bounds.h : 0.f;
            }
            else {
                point.x = SDL_randf() > 0.5f ? bounds.w : 0.f;
                point.y = SDL_randf() * bounds.h;
            }

            // Check if too close to player or other spawn points
            static const XMVECTOR MIN_DIST_SQ = toVec(100.f * 100.f);
            tooClose = false;
            for (const auto& spawn : spawnPoints) {
                const XMVECTOR toSpawn = toVec(spawn) - toVec(point);
                const XMVECTOR distSq = XMVector3LengthSq(toSpawn);
                if (XMVector3Less(distSq, MIN_DIST_SQ)) {
                    tooClose = true;
                    break;
                }
            }
        } while (tooClose);
        spawnPoints.push_back(point);
        enemy.setPos(toVec(point));

        enemy.setColor({ SDL_randf() * 0.5f + 0.5f, 0.0f, 0.0f, 1.f });
        enemies.push_back(enemy);
    }
}

void Scene::update(float dt, float elapsed)
{
    using namespace DirectX;

    if (paused)
        return;

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

        // Check if projectile is out of bounds
        const XMFLOAT2 pos = toFloat2(proj.getPos());
        const float margin = 50.f;

        if (pos.x < margin
            || pos.x > bounds.w - margin
            || pos.y < margin
            || pos.y > bounds.h - margin)
        {
            proj.setHealth(0);
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

    // Spawn enemies
    {
        if (elapsed - wave.lastSpawnTime > wave.spawnInterval) {
            spawnEnemiesOnSceneEdge(wave.enemyCount);
            wave.lastSpawnTime = elapsed;

            // Update parameters
            ++wave.number;
            wave.enemyCount += 10;
            wave.spawnInterval = std::max(5.f, wave.spawnInterval * 0.9f);
        }
    }
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
    {
        SDL_FRect healthBar{
            scaledBounds.x + 20.f * uiScale,
            scaledBounds.y + 20.f * uiScale,
            200.f * uiScale,
            20.f * uiScale
        };

        // Background
        SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
        SDL_RenderFillRect(renderer, &healthBar);
        // Foreground
        const float healthPercent = player.getHealthPercent();
        healthBar.w *= healthPercent;
        SDL_SetRenderDrawColor(renderer, 30, 200, 30, 255);
        SDL_RenderFillRect(renderer, &healthBar);

        // Health text
        char healthText[32];
        SDL_snprintf(healthText, sizeof(healthText), "%d / %d",
            player.getHealth(), player.getMaxHealth());
        const float textScale = uiScale;
        SDL_SetRenderScale(renderer, textScale, textScale);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        const float textX = (scaledBounds.x
            + 20.f * uiScale
            + 100.f * uiScale
            - SDL_strlen(healthText) * 4.f * uiScale)
            / textScale;
        const float textY = (scaledBounds.y + 22.f * uiScale) / textScale;
        SDL_RenderDebugText(renderer, textX, textY, healthText);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }

    // Wave number
    {
        char waveText[32];
        SDL_snprintf(waveText, sizeof(waveText), "Wave %d", wave.number);
        const float textScale = uiScale;
        SDL_SetRenderScale(renderer, textScale, textScale);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        const float textX = (scaledBounds.x + scaledBounds.w * 0.5f
            - SDL_strlen(waveText) * 4.f * uiScale) / textScale;
        const float textY = (scaledBounds.y + 22.f * uiScale) / textScale;
        SDL_RenderDebugText(renderer, textX, textY, waveText);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}
