#include "GameEntity.h"
using namespace DirectX;

GameEntity::GameEntity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, EntityType _entityType, float length, float width, float height)
{
    mesh = _mesh;
	material = _material;
	entityType = _entityType;

	collider = std::make_shared<AABB>(GetTransform(), length, width, height, true);
	transform.SetScale(width, height, length);
}

GameEntity::~GameEntity()
{
}

std::shared_ptr<Mesh> GameEntity::GetMesh() { return mesh; }

std::shared_ptr<Material> GameEntity::GetMaterial() { return material; }

void GameEntity::SetMaterial(std::shared_ptr<Material> _material) { material = _material; }

Transform* GameEntity::GetTransform() { return &transform; }

void GameEntity::Update(float deltaTime)
{
	collider->SetCenter(transform.GetPosition());
}

void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	material->PrepareMaterial(&transform, camera);

	// Draw the mesh
	mesh->Draw(context);
}
