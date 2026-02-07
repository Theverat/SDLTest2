#pragma once

#include <DirectXMath.h>

class Object;

inline DirectX::XMVECTOR toVec(float x)
{
    return DirectX::XMVectorReplicate(x);
}

inline DirectX::XMVECTOR toVec(float x, float y)
{
    return DirectX::XMVectorSet(x, y, 0.0f, 0.0f);
}

inline DirectX::XMVECTOR toVec(float x, float y, float z)
{
    return DirectX::XMVectorSet(x, y, z, 0.0f);
}

inline DirectX::XMVECTOR toVec(float x, float y, float z, float w)
{
    return DirectX::XMVectorSet(x, y, z, w);
}

inline DirectX::XMVECTOR toVec(const DirectX::XMFLOAT2& f)
{
    return DirectX::XMLoadFloat2(&f);
}

inline DirectX::XMVECTOR toVec(const DirectX::XMFLOAT3& f)
{
    return DirectX::XMLoadFloat3(&f);
}

inline DirectX::XMVECTOR toVec(const DirectX::XMFLOAT4& f)
{
    return DirectX::XMLoadFloat4(&f);
}

inline DirectX::XMFLOAT2 toFloat2(const DirectX::XMVECTOR& v)
{
    DirectX::XMFLOAT2 f;
    DirectX::XMStoreFloat2(&f, v);
    return f;
}

inline DirectX::XMFLOAT3 toFloat3(const DirectX::XMVECTOR& v)
{
    DirectX::XMFLOAT3 f;
    DirectX::XMStoreFloat3(&f, v);
    return f;
}

inline DirectX::XMFLOAT4 toFloat4(const DirectX::XMVECTOR& v)
{
    DirectX::XMFLOAT4 f;
    DirectX::XMStoreFloat4(&f, v);
    return f;
}

bool checkCollision(const Object& obj1, const Object& obj2);

inline bool isOutOfBounds(DirectX::XMVECTOR pos,
    DirectX::XMVECTOR min,
    DirectX::XMVECTOR max)
{
    return DirectX::XMVector3Less(pos, min)
        || DirectX::XMVector3Greater(pos, max);
}

inline DirectX::XMVECTOR clamp(DirectX::XMVECTOR value,
    DirectX::XMVECTOR min,
    DirectX::XMVECTOR max)
{
    return DirectX::XMVectorMax(min, DirectX::XMVectorMin(max, value));
}
