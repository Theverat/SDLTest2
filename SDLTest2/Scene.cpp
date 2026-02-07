#include "Scene.h"

using namespace DirectX;

Scene::Scene()
{
    restart();
}

void Scene::spawnEnemiesOnSceneEdge(int enemyCount)
{
    assert(enemyCount > 0);
    SDL_Log("Spawning %d enemies", enemyCount);
    for (int i = 0; i < enemyCount; ++i) {
        Enemy enemy{};
        // Random point at the edge of the scene
        XMFLOAT2 point{};
        if (SDL_randf() > 0.5f) {
            point.x = SDL_randf() * bounds.w;
            point.y = SDL_randf() > 0.5f ? bounds.h : 0.f;
        }
        else {
            point.x = SDL_randf() > 0.5f ? bounds.w : 0.f;
            point.y = SDL_randf() * bounds.h;
        }
        enemy.setPos(toVec(point));


        if (i < wave.smartEnemyCount) {
            enemy.setAIMode(Enemy::AIMode::PREDICTIVE_CHASE);
            enemy.setColor({ 1.0f, 0.f, 0.f, 1.f });
            enemy.setMaxSpeed(200.f);
            enemy.setAcceleration(500.f);
        }
        else {
            // Dumb enemy
            enemy.setAIMode(Enemy::AIMode::DUMB_CHASE);
            enemy.setColor({ 0.f, 1.f, 1.f, 1.f });
        }

        enemies.push_back(enemy);
    }
}

void Scene::restart()
{
    player = Player{};
    player.setPos({ bounds.w / 2.f, bounds.h / 2.f, 0.0f });
    player.setColor({ 0.0f, 1.0f, 0.0f, 1.f });

    enemies.clear();
    projectiles.clear();
    wave = Wave{};
    stats = Stats{};
    paused = false;
}

void Scene::update(float dt, float elapsed)
{
    using namespace DirectX;
    playerTookDamage = false;

    if (paused)
        return;

    // Game over?
    if (isGameOver())
        return;

    // Updates

    const float margin = 10.f;
    const XMVECTOR minPos = toVec(bounds.x + margin, bounds.y + margin);
    const XMVECTOR maxPos = toVec(bounds.w - margin, bounds.h - margin);

    player.update(dt, elapsed);
    player.clampPos(minPos, maxPos);
    player.maybeFire(*this, elapsed);

    for (auto& enemy : enemies) {
        enemy.doAIBehavior(*this, dt, elapsed);
        enemy.update(dt, elapsed);
        enemy.clampPos(minPos, maxPos);

        if (checkCollision(enemy, player)) {
            if (enemy.maybeApplyDamage(player, elapsed)) {
                playerTookDamage = true;
            }
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

        // Delete if out of bounds
        if (isOutOfBounds2D(proj.getPos(), minPos, maxPos))
        {
            proj.setColor({ 0.f, 0.f, 1.f, 1.f });
            proj.setHealth(0);
        }
    }

    // Delete dead objects

    const size_t oldEnemyCount = enemies.size();
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& enemy) {
            return !enemy.isAlive();
        }), enemies.end());
    stats.enemiesKilled += oldEnemyCount - enemies.size();

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& proj) {
            return !proj.isAlive();
        }), projectiles.end());

    // Spawn enemies
    {
        if (elapsed - wave.lastSpawnTime > wave.spawnInterval) {
            spawnEnemiesOnSceneEdge(wave.enemyCount);
            wave.lastSpawnTime = elapsed;

            // Update wave parameters
            ++wave.counter;
            wave.enemyCount += 20;
            wave.smartEnemyCount += 3;
            wave.spawnInterval = std::max(5.f, wave.spawnInterval * 0.9f);
        }
    }
}

void Scene::draw(SDL_Renderer* renderer, float elapsed,
    int width, int height, float uiScale)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

    enum class Align {
        LEFT,
        CENTER,
    };
    auto drawText = [&](const char* text, float xPerc, float yPerc,
        float scale = 1.f,
        Align align = Align::LEFT)
        {
            const float textScale = uiScale * scale;
            SDL_SetRenderScale(renderer, textScale, textScale);
            float x = scaledBounds.x + scaledBounds.w * xPerc;
            const float y = scaledBounds.y + scaledBounds.h * yPerc;
            if (align == Align::CENTER)
                x -= SDL_strlen(text) * 6.7f * scale;
            SDL_RenderDebugText(renderer, x / textScale, y / textScale, text);
            SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        };


    // TODO do the same for other sprites/objects?
    // -> use SDL_SetRenderScale()?

    // Draw ground
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &scaledBounds);

    // Draw objects
    const XMVECTOR offset = toVec(scaledBounds.x, scaledBounds.y);
    const XMVECTOR scale = toVec(scaledBounds.w / bounds.w,
        scaledBounds.h / bounds.h);

    player.draw(renderer, offset, scale);
    for (auto& obj : enemies) {
        obj.draw(renderer, offset, scale);
    }
    for (auto& proj : projectiles) {
        proj.draw(renderer, offset, scale);
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
        SDL_SetRenderDrawColor(renderer, 30, 150, 30, 255);
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
        const float textY = (scaledBounds.y + 24.f * uiScale) / textScale;
        SDL_RenderDebugText(renderer, textX, textY, healthText);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }

    // Wave number
    {
        char waveText[64];
        SDL_snprintf(waveText, sizeof(waveText),
            "Wave %d (next in %.1f s)",
            wave.counter,
            std::abs(elapsed - (wave.lastSpawnTime + wave.spawnInterval)));
        const float textScale = uiScale;
        SDL_SetRenderScale(renderer, textScale, textScale);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        const float textX = (scaledBounds.x + scaledBounds.w * 0.5f
            - SDL_strlen(waveText) * 4.f * uiScale) / textScale;
        const float textY = (scaledBounds.y + 22.f * uiScale) / textScale;
        SDL_RenderDebugText(renderer, textX, textY, waveText);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }

    // Game over screen
    if (isGameOver()) {
        const float marginFac = 0.2f;
        SDL_FRect rect{
            scaledBounds.x + scaledBounds.w * marginFac,
            scaledBounds.y + scaledBounds.h * marginFac,
            scaledBounds.w * (1.f - 2.f * marginFac),
            scaledBounds.h * (1.f - 2.f * marginFac),
        };
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 200);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawText("Game Over", 0.5f, 0.3f, 8.f, Align::CENTER);

        char text[64];
        SDL_snprintf(text, sizeof(text),
            "Waves defeated: %d",
            wave.counter - 1);
        drawText(text, 0.5f, 0.45f, 4.f, Align::CENTER);

        SDL_snprintf(text, sizeof(text),
            "Enemies killed: %d",
            stats.enemiesKilled);
        drawText(text, 0.5f, 0.52f, 4.f, Align::CENTER);

        drawText("Press A to restart", 0.5f, 0.7f, 2.f, Align::CENTER);
    }

    // Debug info
    {
        char text[64];
        SDL_snprintf(text, sizeof(text),
            "Projectiles: %d",
            projectiles.size());
        SDL_SetRenderScale(renderer, uiScale, uiScale);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer,
            scaledBounds.x + 20.f * uiScale,
            scaledBounds.y + 30.f * uiScale,
            text);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }

    // Damage indicator
    if (playerTookDamage) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
        SDL_RenderFillRect(renderer, &scaledBounds);
    }
}
