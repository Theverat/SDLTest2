#pragma once
#include "Object.h"

#include <SDL3/SDL.h>

class Scene;

class Player : public Object
{
public:
    void handleInput(const SDL_Event& event, Scene& scene);
    void fire(Scene& scene);

private:

};

