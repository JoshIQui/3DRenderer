#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "AABB.h"

enum EntityType 
{
	Death,
	Pickup
};

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, EntityType _entityType, float length, float width, float height);
	~GameEntity();

	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> _material);

	std::shared_ptr<AABB> collider;
	EntityType entityType;

	Transform* GetTransform();

	void Update(float deltaTime);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera);
private:
	Transform transform;

	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
};

