#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex()
	{
		position = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f };
		uv = DirectX::XMFLOAT2{ 0.0f, 0.0f };
		normal = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;

	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v, FLOAT nx, FLOAT ny, FLOAT nz) : position(x, y, z), uv(u, v), normal(nx, ny, nz) {}
};


