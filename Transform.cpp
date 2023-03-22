#include "Transform.h"
using namespace DirectX;

Transform::Transform() : 
    position(0,0,0),
    pitchYawRoll(0,0,0),
    scale(1,1,1),
    up(0, 1, 0),
    forward(0, 0, 1),
    right(1, 0, 0)
{
    XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&worldInverseTransposeMatrix, XMMatrixIdentity());
    matrixDirty = false;
}

Transform::~Transform()
{
}

void Transform::MoveAbsolute(float x, float y, float z)
{
    lastPosition = position;

    //Math library way
    XMVECTOR pos = XMLoadFloat3(&position);
    XMVECTOR offset = XMVectorSet(x, y, z, 0);
    XMStoreFloat3(&position, pos + offset);
    matrixDirty = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
    lastPosition = position;
    // Set the offset to a variable and get the current rotation
    XMVECTOR offset = XMVectorSet(x, y, z, 0);
    XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);

    // Apply the rotation to the offset
    XMVECTOR dir = XMVector3Rotate(offset, rotation);

    // Apply the offset to the position
    XMStoreFloat3(&position, XMLoadFloat3(&position) + dir);

    matrixDirty = true;
}

void Transform::Rotate(float p, float y, float r)
{
    pitchYawRoll.x += p;
    pitchYawRoll.y += y;
    pitchYawRoll.z += r;
    matrixDirty = true;
    UpdateVectors();
}

void Transform::Scale(float x, float y, float z)
{
    scale.x *= x;
    scale.y *= y;
    scale.z *= z;
    matrixDirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    matrixDirty = true;
}

void Transform::SetPitchYawRoll(float p, float y, float r)
{
    pitchYawRoll.x = p;
    pitchYawRoll.y = y;
    pitchYawRoll.z = r;
    matrixDirty = true;
    UpdateVectors();
}

void Transform::SetScale(float x, float y, float z)
{
    scale.x = x;
    scale.y = y;
    scale.z = z;
    matrixDirty = true;
}

DirectX::XMFLOAT3 Transform::GetPosition() { return position; }

DirectX::XMFLOAT3 Transform::GetPitchYawRoll() { return pitchYawRoll; }

DirectX::XMFLOAT3 Transform::GetScale() { return scale; }

DirectX::XMFLOAT3 Transform::GetUp() { return up; }

DirectX::XMFLOAT3 Transform::GetForward() { return forward; }

DirectX::XMFLOAT3 Transform::GetRight() { return right; }

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
    if (matrixDirty) {
        // Build the matrices representing each type of transform
        //XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z); Easy Way
        XMMATRIX trans = XMMatrixTranslationFromVector(XMLoadFloat3(&position));

        XMMATRIX rot = XMMatrixRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);

        XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

        // Combine them
        XMMATRIX worldMat = sc * rot * trans; // S * R * T

        // Store as a storage type and return
        XMStoreFloat4x4(&worldMatrix, worldMat);
        XMStoreFloat4x4(
            &worldInverseTransposeMatrix,
            XMMatrixInverse(0, XMMatrixTranspose(worldMat)));

        matrixDirty = false;
    }
    return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseTransposeMatrix()
{
    return GetWorldMatrix();
}

DirectX::XMVECTOR Transform::GetPositionVector()
{
    return XMLoadFloat3(&position);
}

void Transform::UpdateVectors()
{
    // Get the current rotation of the transform
    XMVECTOR rotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

    // Set the directions of the transform
    XMStoreFloat3(&up, XMVector3Rotate(XMVectorSet(0, 1, 0, 0), rotation));      // Up
    XMStoreFloat3(&forward, XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotation)); // Forward
    XMStoreFloat3(&right, XMVector3Rotate(XMVectorSet(1, 0, 0, 0), rotation));   // Right
}
