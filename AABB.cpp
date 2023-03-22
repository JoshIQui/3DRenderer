#include "AABB.h"
#include <iostream>

AABB::AABB(Transform* transform, std::vector<Vertex> verts, int numVerts, bool isActive)
{
	this->isActive = isActive;

	min = verts[0].Position;
	max = verts[0].Position;

	for (int i = 1; i < numVerts; i++)
	{
		min.x = fmin(verts[i].Position.x, min.x);
		max.x = fmax(verts[i].Position.x, max.x);

		min.y = fmin(verts[i].Position.y, min.y);
		max.y = fmax(verts[i].Position.y, max.y);

		min.z = fmin(verts[i].Position.z, min.z);
		max.z = fmax(verts[i].Position.z, max.z);
	}

	SetCenter(transform->GetPosition());
}

AABB::AABB(Transform* transform, float length, float width, float height, bool isActive)
{
	this->isActive = isActive;

	float x = width / 2;
	float y = height / 2;
	float z = length / 2;

	min = DirectX::XMFLOAT3(-x, -y, -z);
	max = DirectX::XMFLOAT3(x, y, z);

	SetCenter(transform->GetPosition());
}

void AABB::SetCenter(DirectX::XMFLOAT3 position)
{
	center = position;

	minG.x = min.x + center.x;
	maxG.x = max.x + center.x;

	minG.y = min.y + center.y;
	maxG.y = max.y + center.y;

	minG.z = min.z + center.z;
	maxG.z = max.z + center.z;
}

bool AABB::CheckCollision(std::shared_ptr<AABB> other)
{
	if (minG.x <= other->maxG.x &&
		maxG.x >= other->minG.x &&
		minG.y <= other->maxG.y &&
		maxG.y >= other->minG.y &&
		minG.z <= other->maxG.z &&
		maxG.z >= other->minG.z)
	{
		return true;
	}

	return false;
}
