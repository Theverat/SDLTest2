#include "Object.h"

using namespace DirectX;

void Object::draw(SDL_Renderer* renderer, XMVECTOR offset, XMVECTOR scale)
{
    // Body
    const XMVECTOR r = toVec(radius) * scale;
    const XMVECTOR screenPos = getPos() * scale + offset;

    const XMFLOAT2 topLeft = toFloat2(screenPos - r);
    const XMFLOAT2 size = toFloat2(r * toVec(2.f));
    SDL_FRect rect{ topLeft.x,
                    topLeft.y,
                    size.x,
                    size.y };
    SDL_SetRenderDrawColor(renderer,
        Uint8(color.x * 255.f),
        Uint8(color.y * 255.f),
        Uint8(color.z * 255.f),
        Uint8(color.w * 255.f));
    SDL_RenderFillRect(renderer, &rect);

    // Aim direction
    const XMVECTOR lineLength = toVec(100.f) * scale;

    const XMFLOAT2 lineStart = toFloat2(screenPos);
    const XMFLOAT2 lineEnd = toFloat2(screenPos + getAimDir() * lineLength);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderLine(renderer,
        lineStart.x,
        lineStart.y,
        lineEnd.x,
        lineEnd.y);
}
