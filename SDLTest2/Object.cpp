#include "Object.h"

using namespace DirectX;

void Object::draw(SDL_Renderer* renderer)
{
    // Body
    SDL_SetRenderDrawColor(renderer,
        Uint8(color.x * 255.f),
        Uint8(color.y * 255.f),
        Uint8(color.z * 255.f),
        Uint8(color.w * 255.f));

    // TODO make resolution independent
    const XMFLOAT3 p = toFloat3(getPos());
    SDL_FRect rect{ p.x - radius,
                    p.y - radius,
                    radius * 2.f,
                    radius * 2.f };
    SDL_RenderFillRect(renderer, &rect);

    // Aim direction
    const XMFLOAT3 aimDirFloat = toFloat3(getAimDir());
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderLine(renderer,
        p.x,
        p.y,
        p.x + aimDirFloat.x * 60,
        p.y + aimDirFloat.y * 60);
}
