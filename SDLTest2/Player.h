#pragma once
#include "Object.h"

#include <SDL3/SDL.h>

class Scene;

class Player : public Object
{
public:
    void handleInput(const SDL_Event& event, Scene& scene);
    void maybeFire(Scene& scene, float elapsed);

private:
    static constexpr float FIRE_INTERVAL{ 0.05f };
    float lastFireTime{};
    bool firing{ false };
};

