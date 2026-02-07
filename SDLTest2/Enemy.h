#pragma once
#include "Object.h"

#include <DirectXMath.h>

class Scene;

class Enemy : public Object
{
public:
    enum class AIMode {
        DUMB_CHASE,
        PREDICTIVE_CHASE,
    };

    void setAIMode(AIMode mode) {
        aiMode = mode;
    }
    void doAIBehavior(const Scene& scene, float dt, float elapsed);

    // Note: does not check if the objects collide!
    bool maybeApplyDamage(Object& obj, float elapsed) {
        if (elapsed - lastDamageTime < DAMAGE_INTERVAL) {
            return false;
        }
        obj.setHealth(obj.getHealth() - damage);
        lastDamageTime = elapsed;
        return true;
    }

    void setDamage(int value) {
        damage = value;
    }

    int getDamage() const {
        return damage;
    }

    void setAcceleration(float value) {
        acceleration = value;
    }

    void setMaxSpeed(float value) {
        maxSpeed = value;
    }

private:
    void doDumbChase(const Scene& scene, float dt, float elapsed);
    void doPredictiveChase(const Scene& scene, float dt, float elapsed);

    AIMode aiMode{ AIMode::DUMB_CHASE };
    float acceleration{ 80.f };
    float maxSpeed{ 300.f };

    static constexpr float DAMAGE_INTERVAL{ 0.5f };
    int damage{ 5 };
    float lastDamageTime{};
};
