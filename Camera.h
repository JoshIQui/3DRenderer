#pragma once
#include <DirectXMath.h>
#include "Input.h"
#include "Transform.h"

// Using an enum for the projection seems better than a boolean
enum class Projection
{
	Perspective,
	Orthographic
};

class Camera
{
public:
	Camera(
		DirectX::XMFLOAT3 _position,
		float _fov,
		float _aspectRatio,
		float _nearClipDistance,
		float _farClipDistance,
		float _movementSpeed,
		float _lookSpeed,
		Projection _projection = Projection::Perspective
	);
	~Camera();

	//*************************************
	// Getting Functions
	Transform* GetTransform();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	float GetFOV();
	float GetAspectRatio();
	float GetNearClipDistance();
	float GetFarClipDistance();
	float GetMovementSpeed();
	float GetLookSpeed();
	Projection GetProjection();
	//*************************************

	//*************************************
	// Setting Functions
	void SetFOV(float value);
	void SetNearClipDistance(float value);
	void SetFarClipDistance(float value);
	void SetMovementSpeed(float value);
	void SetLookSpeed(float value);
	void SetProjection(Projection _projection);
	void SetTransform(Transform _transform);
	//*************************************

	void Update(float _deltaTime);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float _aspectRatio);

private:
	Transform transform;

	float fov;
	float aspectRatio;
	float nearClipDistance;
	float farClipDistance;

	float movementSpeed;
	float lookSpeed;

	Projection projection;

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

