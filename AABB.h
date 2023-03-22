#pragma once
#include <DirectXMath.h>
#include "Vertex.h"
#include "Transform.h"
#include <memory>
#include <vector>

class AABB
{
public:
	AABB(Transform* transform, std::vector<Vertex> verts, int numVerts, bool isActive);
	AABB(Transform* transform, float length, float width, float height, bool isActive);

	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 minG;

	DirectX::XMFLOAT3 max;
	DirectX::XMFLOAT3 maxG;

	DirectX::XMFLOAT3 center;

	void SetCenter(DirectX::XMFLOAT3 position);

	bool CheckCollision(std::shared_ptr<AABB> other);

	bool isActive;

private:
};

