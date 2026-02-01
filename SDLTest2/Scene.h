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
            objects.push_back(obj);
        }

        player.setPos({ 200.0f, 100.0f, 0.0f });
        player.setColor({ 0.0f, 1.0f, 0.0f, 1.f });
    }

    ~Scene() {}

    void handleInput(const SDL_Event& event) {
        player.handleInput(event, *this);
    }

    void update(float dt) {
        player.update(dt);
        for (auto& obj : objects) {
            obj.update(dt);
        }
        for (auto& proj : projectiles) {
            proj.update(dt);
        }
    }

    void draw(SDL_Renderer* renderer) {
        player.draw(renderer);
        for (auto& obj : objects) {
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
    std::vector<Object> objects;
    std::vector<Projectile> projectiles;
    Player player{};
};

