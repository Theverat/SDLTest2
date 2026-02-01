#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Scene.h"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

static SDL_FRect mouseposrect;

// Game objects
static Scene* scene = nullptr;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("SDL Hello World Example", "1.0", "com.example.sdl-hello-world");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 2000, 2000, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    mouseposrect.x = mouseposrect.y = -1000;  /* -1000 so it's offscreen at start */
    mouseposrect.w = mouseposrect.h = 50;

    // Init game objects
    scene = new Scene();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    static Uint64 last = 0;
    const Uint64 now = SDL_GetPerformanceCounter();
    const float dt = (now - last) / float(SDL_GetPerformanceFrequency());

    scene->update(dt);
    scene->draw(renderer);

    SDL_RenderPresent(renderer);

    last = now;
    return SDL_APP_CONTINUE;

}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:  /* triggers on last window close and other things. End the program. */
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:  /* quit if user hits ESC key */
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:  /* keep track of the latest mouse position */
        /* center the square where the mouse is */
        mouseposrect.x = event->motion.x - (mouseposrect.w / 2);
        mouseposrect.y = event->motion.y - (mouseposrect.h / 2);
        break;
    }

    scene->handleInput(*event);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
