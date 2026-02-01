#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Scene.h"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Gamepad* gamepad = nullptr;

// Game objects
static Scene* scene = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("SDL Test2", "1.0", "com.example.sdl-test2");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 2200, 1200,
        SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    int width{}, height{};
    SDL_GetWindowSizeInPixels(window, &width, &height);
    SDL_Log("Window size: %dx%d", width, height);
    const SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
    const float displayScale = SDL_GetDisplayContentScale(displayID);
    SDL_Log("Display scale: %.2f", displayScale);

    // Init game objects
    scene = new Scene();

    // Open first available gamepad
    int num_joysticks = 0;
    SDL_JoystickID* joysticks = SDL_GetGamepads(&num_joysticks);
    if (joysticks) {
        SDL_Log("Found %d gamepad(s)", num_joysticks);
        if (num_joysticks > 0) {
            gamepad = SDL_OpenGamepad(joysticks[0]);
            if (gamepad) {
                SDL_Log("Gamepad connected: %s", SDL_GetGamepadName(gamepad));
            }
            else {
                SDL_Log("Failed to open gamepad: %s", SDL_GetError());
            }
        }
        SDL_free(joysticks);
    }
    else {
        SDL_Log("SDL_GetGamepads failed: %s", SDL_GetError());
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    static Uint64 last = 0;
    const Uint64 now = SDL_GetPerformanceCounter();
    if (last == 0) {
        last = now;
    }

    const float dt = (now - last) / float(SDL_GetPerformanceFrequency());
    const float elapsed = now / float(SDL_GetPerformanceFrequency());

    int width{}, height{};
    SDL_GetWindowSizeInPixels(window, &width, &height);
    const SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
    const float uiScale = SDL_GetDisplayContentScale(displayID);

    scene->update(dt, elapsed);
    scene->draw(renderer, elapsed, width, height, uiScale);

    SDL_RenderPresent(renderer);

    last = now;
    return SDL_APP_CONTINUE;

}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:  /* quit if user hits ESC key */
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        break;
    }

    scene->handleInput(*event);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (gamepad) {
        SDL_CloseGamepad(gamepad);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
