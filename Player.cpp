#include "Player.h"
#include <iostream>
using namespace DirectX;

Player::Player(DirectX::XMFLOAT3 position, std::shared_ptr<Camera> _camera)
{
    transform.SetPosition(position.x, position.y, position.z);

    camera = _camera;
    movementSpeed = 30;
    verticalVelocity = 0;
    score = 0;

    collider = std::make_shared<AABB>(GetTransform(), 10, 10, 10, false);
}

void Player::Update(float deltaTime)
{
    Input& input = Input::GetInstance();

    camera->GetTransform()->SetPosition(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
    camera->Update(deltaTime);

    float speed = movementSpeed * deltaTime;

    // Clamp
    XMFLOAT3 pos = transform.GetPosition();

    transform.SetPitchYawRoll(0, camera->GetTransform()->GetPitchYawRoll().y, 0);

    // WASD Controls
    if (input.KeyDown('W')) // Move Forward
    { 
        transform.MoveRelative(0, 0, speed);
        camera->GetTransform()->MoveRelative(0, 0, speed);
    }
    if (input.KeyDown('A')) // Move Left
    {
        transform.MoveRelative(-speed, 0, 0);
        camera->GetTransform()->MoveRelative(-speed, 0, 0);
    }
    if (input.KeyDown('S')) // Move Backward
    {
        transform.MoveRelative(0, 0, -speed);
        camera->GetTransform()->MoveRelative(0, 0, -speed);
    }
    if (input.KeyDown('D')) // Move Right
    {
        transform.MoveRelative(speed, 0, 0);
        camera->GetTransform()->MoveRelative(speed, 0, 0);
    }

    // Vertical Controls
    if (input.KeyDown(VK_SPACE)) { verticalVelocity = 50.0f; }// Jump

    transform.MoveAbsolute(0, verticalVelocity * deltaTime, 0);
    camera->GetTransform()->MoveAbsolute(0, verticalVelocity * deltaTime, 0);

    verticalVelocity -= 100.0f * deltaTime;

    if (verticalVelocity < -50.0f) { verticalVelocity = -50.0f; }

    collider->SetCenter(transform.GetPosition());
}

Transform* Player::GetTransform() { return &transform; }

void Player::CheckTerrainCollision(std::shared_ptr<GameEntity> other, Transform* otherTransform)
{
    if (collider->CheckCollision(other->collider))
    {
        switch (other->entityType)
        {
            case Death:
                Respawn();
                break;

            case Pickup:
                score += 5;
                other->collider->isActive = false;
                break;
        }
    }
}

void Player::Respawn()
{
    transform.SetPosition(0, 0, 0);
    camera->GetTransform()->SetPosition(0, 0, 0);
}
