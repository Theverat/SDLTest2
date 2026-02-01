#pragma once
#include "Object.h"

#include <SDL3/SDL.h>

class Player : public Object
{
public:
    void handleInput(const SDL_Event& event) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.scancode) {
            case SDL_SCANCODE_W:
                setSpeed({ 0.0f, -200.0f, 0.0f });
                break;
            case SDL_SCANCODE_S:
                setSpeed({ 0.0f, 200.0f, 0.0f });
                break;
            case SDL_SCANCODE_A:
                setSpeed({ -200.0f, 0.0f, 0.0f });
                break;
            case SDL_SCANCODE_D:
                setSpeed({ 200.0f, 0.0f, 0.0f });
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_EVENT_KEY_UP) {
            switch (event.key.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                //setSpeed({ 0.0f, 0.0f, 0.0f });
                break;
            default:
                break;
            }
        }
    }
private:

};

