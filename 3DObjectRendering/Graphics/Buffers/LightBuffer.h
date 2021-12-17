#pragma once
#include "DirectXMath.h"

struct LightBuffer
{
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDirection;
	FLOAT specularPower;
	DirectX::XMFLOAT4 specularColor;
};
