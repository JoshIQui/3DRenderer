#pragma once
#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	~Transform();
	//*************************************
	// Offsetting Methods
	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void Rotate(float p, float y, float r);
	void Scale(float x, float y, float z);
	//*************************************

	//*************************************
	// Setting Functions
	void SetPosition(float x, float y, float z);
	void SetPitchYawRoll(float p, float y, float r);
	void SetScale(float x, float y, float z);
	//*************************************

	//*************************************
	// Getting Functions
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetPitchYawRoll();
	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT3 GetUp();
	DirectX::XMFLOAT3 GetForward();
	DirectX::XMFLOAT3 GetRight();

	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldInverseTransposeMatrix();

	DirectX::XMVECTOR GetPositionVector();
	//*************************************

	void UpdateVectors();
private:
	//*************************************
	// Raw Transformation Data
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 lastPosition;
	DirectX::XMFLOAT3 pitchYawRoll; // Not a quaternion
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 forward;
	DirectX::XMFLOAT3 right;

	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 worldInverseTransposeMatrix;
	//*************************************

	bool matrixDirty;
};

