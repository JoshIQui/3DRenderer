#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Mesh.h"
#include <vector>
#include <memory>
#include "Transform.h"
#include "GameEntity.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Material.h"
#include "Lights.h"
#include "WICTextureLoader.h"
#include "Sky.h"
#include "Player.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders(); 
	void CreateGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Vector to hold list of GameEntities
	std::vector<std::shared_ptr<GameEntity>> environmentEntities;
	std::vector<std::shared_ptr<GameEntity>> collectibleEntities;

	// Shared pointer to a camera object
	std::shared_ptr<Camera> camera;

	std::shared_ptr<Player> player;

	// A vertex shader constant buffer
	//Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer;
	
	// Shaders and shader-related constructs
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> defaultPixelShader;
	std::shared_ptr<SimplePixelShader> customPixelShader;

	std::shared_ptr<Sky> sky;
	std::shared_ptr<SimpleVertexShader> skyVertexShader;
	std::shared_ptr<SimplePixelShader> skyPixelShader;

	float timeWindow;
	bool timeIncreasing;

	DirectX::XMFLOAT3 ambientColor;

	std::vector<Light> lights;
};

