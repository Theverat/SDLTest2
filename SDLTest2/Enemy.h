#pragma once
#include "Object.h"

#include <DirectXMath.h>

class Scene;

class Enemy : public Object
{
public:
    void doAIBehavior(const Scene& scene, float dt, float elapsed);

    // Note: does not check if the objects collide!
    void maybeApplyDamage(Object& obj, float elapsed) {
        if (elapsed - lastDamageTime < DAMAGE_INTERVAL) {
            return;
        }
        obj.setHealth(obj.getHealth() - damage);
        lastDamageTime = elapsed;
    }

    void setDamage(int value) {
        damage = value;
    }

    int getDamage() const {
        return damage;
    }

private:
    static constexpr float MAX_SPEED_F{ 100.f };
    static constexpr DirectX::XMVECTOR MAX_SPEED{
        MAX_SPEED_F,
        MAX_SPEED_F,
        MAX_SPEED_F,
        0.f
    };

    static constexpr float DAMAGE_INTERVAL{ 0.5f };
    int damage{ 1 };
    float lastDamageTime{};
};
