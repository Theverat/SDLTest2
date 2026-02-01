#pragma once

#include <DirectXMath.h>
#include <SDL3/SDL.h>

class Object
{
public:
    void setPos(DirectX::XMFLOAT3 p)
    {
        pos = DirectX::XMLoadFloat3(&p);
    }

    DirectX::XMFLOAT3 getPos() const
    {
        DirectX::XMFLOAT3 p;
        DirectX::XMStoreFloat3(&p, pos);
        return p;
    }

    void setSpeed(DirectX::XMFLOAT3 s)
    {
        speed = DirectX::XMLoadFloat3(&s);
    }

    DirectX::XMFLOAT3 getSpeed() const
    {
        DirectX::XMFLOAT3 s;
        DirectX::XMStoreFloat3(&s, speed);
        return s;
    }

    void setColor(DirectX::XMFLOAT4 c)
    {
        color = c;
    }

    DirectX::XMFLOAT4 getColor() const
    {
        return color;
    }

    void update(float dt)
    {
        using namespace DirectX;

        XMFLOAT3 speed2;
        XMStoreFloat3(&speed2, speed);
        //if (speed2.x != 0 || speed2.y != 0)
        //    __debugbreak();

        pos += speed * dt;
    }

    void draw(SDL_Renderer* renderer) {
        using namespace DirectX;
        // Draw with SDL3
        // For now, just a circle

        SDL_SetRenderDrawColor(renderer,
            color.x * 255.f,
            color.y * 255.f,
            color.z * 255.f,
            color.w * 255.f);

        const XMFLOAT3 p = getPos();
        SDL_FRect rect{ p.x, p.y, 20, 20 };
        SDL_RenderFillRect(renderer, &rect);
    }

private:
    DirectX::XMVECTOR pos{};
    DirectX::XMVECTOR speed{};
    DirectX::XMFLOAT4 color{};
};
