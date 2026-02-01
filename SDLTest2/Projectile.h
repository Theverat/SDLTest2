#pragma once

#include "Object.h"

class Projectile : public Object
{
public:
    int getDamage() const {
        return damage;
    }

private:
    int damage{ 100 };
};
