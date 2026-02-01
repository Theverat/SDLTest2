#pragma once

#include <DirectXMath.h>
#include <SDL3/SDL.h>

#include "Utils.h"

class Object
{
public:
    Object() = default;
    virtual ~Object() = default;
    Object(const Object&) = default;
    Object& operator=(const Object&) = default;
    Object(Object&&) noexcept = default;
    Object& operator=(Object&&) noexcept = default;

    void setPos(DirectX::XMVECTOR p)
    {
        pos = p;
    }

    DirectX::XMVECTOR getPos() const
    {
        return pos;
    }

    void setSpeed(DirectX::XMVECTOR s)
    {
        speed = s;
    }

    DirectX::XMVECTOR getSpeed() const
    {
        return speed;
    }

    void setColor(DirectX::XMVECTOR c)
    {
        DirectX::XMStoreFloat4(&color, c);
    }

    DirectX::XMVECTOR getColor() const
    {
        return DirectX::XMLoadFloat4(&color);
    }

    void setAimDir(DirectX::XMVECTOR d)
    {
        aimDir = d;
    }

    DirectX::XMVECTOR getAimDir() const
    {
        return aimDir;
    }

    void update(float dt, float elapsed)
    {
        using namespace DirectX;
        pos = XMVectorMultiplyAdd(speed, XMVectorReplicate(dt), pos);
    }

    void draw(SDL_Renderer* renderer) {
        using namespace DirectX;

        // Body
        SDL_SetRenderDrawColor(renderer,
            Uint8(color.x * 255.f),
            Uint8(color.y * 255.f),
            Uint8(color.z * 255.f),
            Uint8(color.w * 255.f));

        // TODO make resolution independent
        const XMFLOAT3 p = toFloat3(getPos());
        SDL_FRect rect{ p.x - radius * 0.5f,
                        p.y - radius * 0.5f,
                        radius, radius };
        SDL_RenderFillRect(renderer, &rect);

        // Aim direction
        const XMFLOAT3 aimDirFloat = toFloat3(getAimDir());
        SDL_RenderLine(renderer,
            p.x,
            p.y,
            p.x + aimDirFloat.x * 30,
            p.y + aimDirFloat.y * 30);
    }

    void setHealth(int h) {
        health = h;
    }

    int getHealth() const {
        return health;
    }

    bool isAlive() const {
        return health > 0;
    }

    void setRadius(float r) {
        radius = r;
    }

    float getRadius() const {
        return radius;
    }

private:
    DirectX::XMVECTOR pos{};
    DirectX::XMVECTOR speed{};
    DirectX::XMVECTOR aimDir{};
    DirectX::XMFLOAT4 color{};
    float radius{ 30.f };

    int health{ 100 };
};
