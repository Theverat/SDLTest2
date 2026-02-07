#include "Utils.h"

#include "Object.h"

using namespace DirectX;

bool checkCollision(const Object& obj1, const Object& obj2)
{
    const XMVECTOR obj1ToObj2 = obj1.getPos() - obj2.getPos();
    const XMVECTOR midpointDistSq = XMVector3LengthSq(obj1ToObj2);
    const XMVECTOR obj1Radius = toVec(obj1.getRadius());
    const XMVECTOR obj2Radius = toVec(obj2.getRadius());

    const XMVECTOR dist = midpointDistSq
        - obj1Radius * obj1Radius
        - obj2Radius * obj2Radius;

    return XMVector3Less(dist, XMVectorZero());
}
