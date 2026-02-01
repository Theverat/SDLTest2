#pragma once

#include <DirectXMath.h>

DirectX::XMVECTOR toVec(const DirectX::XMFLOAT2& f)
{
    return DirectX::XMLoadFloat2(&f);
}

DirectX::XMVECTOR toVec(const DirectX::XMFLOAT3& f)
{
    return DirectX::XMLoadFloat3(&f);
}

DirectX::XMVECTOR toVec(const DirectX::XMFLOAT4& f)
{
    return DirectX::XMLoadFloat4(&f);
}
