#pragma once
#include <memory>
#include "Camera.h"
#include "AABB.h"
#include "GameEntity.h"

class Player
{
public:
	Player(DirectX::XMFLOAT3 position, std::shared_ptr<Camera> _camera);

	void Update(float deltaTime);

	Transform* GetTransform();

	void CheckTerrainCollision(std::shared_ptr<GameEntity> other, Transform* otherTransform);

	std::shared_ptr<AABB> collider;

	void Respawn();
	int score;

private:
	Transform transform;

	std::shared_ptr<Camera> camera;

	float movementSpeed;

	float verticalVelocity;
};

