#include "Camera.h"
using namespace DirectX;

Camera::Camera(DirectX::XMFLOAT3 _position, float _fov, float _aspectRatio, float _nearClipDistance, float _farClipDistance, float _movementSpeed, float _lookSpeed, Projection _projection)
{
    transform.SetPosition(_position.x, _position.y, _position.z);
    fov = _fov;
    aspectRatio = _aspectRatio;
    nearClipDistance = _nearClipDistance;
    farClipDistance = _farClipDistance;
    movementSpeed = _movementSpeed;
    lookSpeed = _lookSpeed;
    projection = _projection;

    UpdateViewMatrix();
    UpdateProjectionMatrix(aspectRatio);
}

Camera::~Camera()
{
}

Transform* Camera::GetTransform() { return &transform; }

DirectX::XMFLOAT4X4 Camera::GetViewMatrix() { return viewMatrix; }

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() { return projectionMatrix; }

float Camera::GetFOV() { return fov; }

float Camera::GetAspectRatio() { return aspectRatio; }

float Camera::GetNearClipDistance() { return nearClipDistance; }

float Camera::GetFarClipDistance() { return farClipDistance; }

float Camera::GetMovementSpeed() { return movementSpeed; }

float Camera::GetLookSpeed() { return lookSpeed; }

Projection Camera::GetProjection() { return projection; }

void Camera::SetFOV(float value) { fov = value; }

void Camera::SetNearClipDistance(float value) { nearClipDistance = value; }

void Camera::SetFarClipDistance(float value) { farClipDistance = value; }

void Camera::SetMovementSpeed(float value) { movementSpeed = value; }

void Camera::SetLookSpeed(float value) { lookSpeed = value; }

void Camera::SetProjection(Projection _projection) { projection = _projection; }

void Camera::SetTransform(Transform _transform)
{
    transform = _transform;
}

void Camera::Update(float _deltaTime)
{
    Input& input = Input::GetInstance();

    //float speed = movementSpeed * _deltaTime;

    //// WASD Controls
    //if (input.KeyDown('W')) { transform.MoveRelative(0, 0, movementSpeed); } // Move Forward
    //if (input.KeyDown('A')) { transform.MoveRelative(-movementSpeed, 0, 0); }// Move Left
    //if (input.KeyDown('S')) { transform.MoveRelative(0, 0, -movementSpeed); }// Move Backward
    //if (input.KeyDown('D')) { transform.MoveRelative(movementSpeed, 0, 0); } // Move Right

    //// Vertical Controls
    //if (input.KeyDown(VK_SPACE)) { transform.MoveAbsolute(0, movementSpeed, 0); }// Move Up
    //if (input.KeyDown('X')) { transform.MoveAbsolute(0, -movementSpeed, 0); }    // Move Down

    //// Speed controls
    //if (input.KeyDown(VK_SHIFT)) { speed *= 5; }
    //if (input.KeyDown(VK_CONTROL)) { speed *= 0.2f; }

    // Mouse Controls
    if (input.MouseLeftDown())
    {
        float xMovement = input.GetMouseXDelta() * lookSpeed * _deltaTime;
        float yMovement = input.GetMouseYDelta() * lookSpeed * _deltaTime;

        transform.Rotate(yMovement, xMovement, 0);

        XMFLOAT3 clampedRotation = transform.GetPitchYawRoll();
        if (clampedRotation.x > XM_PIDIV2) clampedRotation.x = XM_PIDIV2;
        if (clampedRotation.x < -XM_PIDIV2) clampedRotation.x = -XM_PIDIV2;
        transform.SetPitchYawRoll(clampedRotation.x, clampedRotation.y, clampedRotation.z);
    }

    // Update the view matrix every frame
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    XMFLOAT3 position = transform.GetPosition();
    XMFLOAT3 direction = transform.GetForward();

    XMMATRIX newViewMatrix = XMMatrixLookToLH(
        XMLoadFloat3(&position),
        XMLoadFloat3(&direction),
        XMVectorSet(0, 1, 0, 0)); // World up axis

    XMStoreFloat4x4(&viewMatrix, newViewMatrix);
}

void Camera::UpdateProjectionMatrix(float _aspectRatio)
{
    aspectRatio = _aspectRatio;

    if (projection == Projection::Perspective)
    {
        XMMATRIX P = XMMatrixPerspectiveFovLH(
            fov,
            aspectRatio,
            nearClipDistance,
            farClipDistance);

        XMStoreFloat4x4(&projectionMatrix, P);
    }
    else // Orthographic
    {
        XMMATRIX P = XMMatrixOrthographicLH(
            2.0f,
            2.0f / aspectRatio,
            nearClipDistance,
            farClipDistance);

        XMStoreFloat4x4(&projectionMatrix, P);
    }
}
