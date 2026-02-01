#pragma once
#include "Object.h"

#include <DirectXMath.h>

class Scene;

class Enemy : public Object
{
public:
    void doAIBehavior(const Scene& scene, float dt, float elapsed);

private:
    static constexpr float MAX_SPEED_F{ 100.f };
    static constexpr DirectX::XMVECTOR MAX_SPEED{
        MAX_SPEED_F,
        MAX_SPEED_F,
        MAX_SPEED_F,
        0.f
    };
};
