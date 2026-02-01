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

    DirectX::XMVECTOR getPosV() const
    {
        return pos;
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

    void setAimDir(DirectX::XMFLOAT3 d)
    {
        aimDir = DirectX::XMLoadFloat3(&d);
    }

    DirectX::XMFLOAT3 getAimDir() const
    {
        DirectX::XMFLOAT3 d;
        DirectX::XMStoreFloat3(&d, aimDir);
        return d;
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

        // Body
        SDL_SetRenderDrawColor(renderer,
            color.x * 255.f,
            color.y * 255.f,
            color.z * 255.f,
            color.w * 255.f);

        const XMFLOAT3 p = getPos();
        const float size = 20.f;
        SDL_FRect rect{ p.x - size * 0.5f, p.y - size * 0.5f, size, size };
        SDL_RenderFillRect(renderer, &rect);

        // Aim direction
        SDL_RenderLine(renderer,
            static_cast<int>(p.x),
            static_cast<int>(p.y),
            static_cast<int>(p.x + getAimDir().x * 30),
            static_cast<int>(p.y + getAimDir().y * 30));
    }

private:
    DirectX::XMVECTOR pos{};
    DirectX::XMVECTOR speed{};
    DirectX::XMVECTOR aimDir{};
    DirectX::XMFLOAT4 color{};

    int health{ 100 };
};
