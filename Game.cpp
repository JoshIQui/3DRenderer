#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "Helpers.h"
#include <memory>
#include <iostream>

// This code assumes files are in "ImGui" subfolder!
// Adjust as necessary for your own folder structure
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// Direct3D itself, and our window, are not ready at this point!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,			// The application's handle
		L"DirectX Game",	// Text for the window's title bar (as a wide-character string)
		1280,				// Width of the window's client area
		720,				// Height of the window's client area
		false,				// Sync the framerate to the monitor refresh? (lock framerate)
		true),				// Show extra stats (fps) in title bar?
	ambientColor(0.1f, 0.1f, 0.25f)
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Delete all objects manually created within this class
//  - Release() all Direct3D objects created within this class
// --------------------------------------------------------
Game::~Game()
{
	// Call delete or delete[] on any objects or arrays you've
	// created using new or new[] within this class
	// - Note: this is unnecessary if using smart pointers

	// Call Release() on any Direct3D objects made within this class
	// - Note: this is unnecessary for D3D objects stored in ComPtrs

	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// --------------------------------------------------------
// Called once per program, after Direct3D and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// ******************************************************
	// Shared Pointer creation (LECTURE)
	// std::shared_ptr<int> tSmart = std::make_shared<int>();
	// ******************************************************

	// Initialize ImGui itself & platform/renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), context.Get());
	// Pick a style (uncomment one of these 3)
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();


	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		
		//context->IASetInputLayout(inputLayout.Get());

		// Set the active vertex and pixel shaders
		//  - Once you start applying different shaders to different objects,
		//    these calls will need to happen multiple times per frame
		/*context->VSSetShader(vertexShader.Get(), 0, 0);
		context->PSSetShader(pixelShader.Get(), 0, 0);*/
	}

	//// Get size as the next multiple of 16 (instead of hardcoding a size here!)
	//unsigned int size = sizeof(VertexShaderExternalData);
	//size = (size + 15) / 16 * 16; // This will work even if your struct size changes

	//// Describe the constant buffer
	//D3D11_BUFFER_DESC cbDesc = {}; // Sets struct to all zeros
	//cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbDesc.ByteWidth = size; // Must be a multiple of 16
	//cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cbDesc.Usage = D3D11_USAGE_DYNAMIC;

	//// Create the constant buffer
	//device->CreateBuffer(&cbDesc, 0, vsConstantBuffer.GetAddressOf());

	// Create the in-game camera
	camera = std::make_shared<Camera>(
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		45.0f,
		(float)windowWidth / windowHeight,
		0.01f,
		1000,
		0.1f,
		1.0f,
		Projection::Perspective
		);

	player = std::make_shared<Player>(XMFLOAT3(0.0f, 0.0f, 0.0f), camera);

	Light directionalLight1 = {};
	directionalLight1.Type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight1.Direction = XMFLOAT3(1, 0, 0);
	directionalLight1.Color = XMFLOAT3(1, 0.1f, 0.1f);
	directionalLight1.Intensity = 1;
	lights.push_back(directionalLight1);

	Light directionalLight2 = {};
	directionalLight2.Type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight2.Direction = XMFLOAT3(0, -1, 0);
	directionalLight2.Color = XMFLOAT3(0.1f, 0.1f, 1);
	directionalLight2.Intensity = 1;
	lights.push_back(directionalLight2);

	Light directionalLight3 = {};
	directionalLight3.Type = LIGHT_TYPE_DIRECTIONAL;
	directionalLight3.Direction = XMFLOAT3(-1, 0, 0);
	directionalLight3.Color = XMFLOAT3(0.1f, 1, 0.1f);
	directionalLight3.Intensity = 1;
	lights.push_back(directionalLight3);

	Light pointLight1 = {};
	pointLight1.Type = LIGHT_TYPE_POINT;
	pointLight1.Color = XMFLOAT3(1, 0.2f, 0.2f);
	pointLight1.Intensity = 1;
	pointLight1.Position = XMFLOAT3(-10, -3, -2);
	pointLight1.Range = 10;
	lights.push_back(pointLight1);

	Light pointLight2 = {};
	pointLight2.Type = LIGHT_TYPE_POINT;
	pointLight2.Color = XMFLOAT3(1, 1, 1);
	pointLight2.Intensity = 1;
	pointLight2.Position = XMFLOAT3(0, -5, -1);
	pointLight2.Range = 10;
	lights.push_back(pointLight2);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	// Create Shaders
	vertexShader = std::make_shared<SimpleVertexShader>(device, context,
		FixPath(L"VertexShader.cso").c_str());
	defaultPixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"PixelShader.cso").c_str());
	customPixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"CustomPS.cso").c_str());

	skyVertexShader = std::make_shared<SimpleVertexShader>(device, context,
		FixPath(L"SkyVertexShader.cso").c_str());
	skyPixelShader = std::make_shared<SimplePixelShader>(device, context,
		FixPath(L"SkyPixelShader.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateGeometry()
{
	//****************************
	// TODO: SIMPLIFY ALL OF THIS
	//****************************
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestoneTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cobblestoneNormals;
	CreateWICTextureFromFile(device.Get(), context.Get(), FixPath(L"../../Assets/Textures/cobblestone.png").c_str(), nullptr, cobblestoneTexture.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), FixPath(L"../../Assets/Textures/cobblestone_normals.png").c_str(), nullptr, cobblestoneNormals.GetAddressOf());

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cushion;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cushionNormals;
	CreateWICTextureFromFile(device.Get(), context.Get(), FixPath(L"../../Assets/Textures/cushion.png").c_str(), nullptr, cushion.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), FixPath(L"../../Assets/Textures/cushion_normals.png").c_str(), nullptr, cushionNormals.GetAddressOf());

	// Create Sampler
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	D3D11_SAMPLER_DESC samplerDescription = {};
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.MaxAnisotropy = 8;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDescription, samplerState.GetAddressOf());

	//****************************
	// Materials
	//****************************
	std::shared_ptr<Material> material1 = std::make_shared<Material>(vertexShader, defaultPixelShader, XMFLOAT3(1, 1, 1), 0.7f);
	material1->AddShaderResourceView("SurfaceTexture", cobblestoneTexture);
	material1->AddShaderResourceView("NormalMap", cobblestoneNormals);
	material1->AddSamplerState("BasicSampler", samplerState);

	std::shared_ptr<Material> material2 = std::make_shared<Material>(vertexShader, defaultPixelShader, XMFLOAT3(1, 1, 1), 0.7f);
	material2->AddShaderResourceView("SurfaceTexture", cushion);
	material2->AddShaderResourceView("NormalMap", cushionNormals);
	material2->AddSamplerState("BasicSampler", samplerState);

	//****************************
	// 3D Objects
	//****************************
	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cube.obj").c_str(), device);

	//****************************
	// Environment
	//****************************
#pragma region Environment
	environmentEntities.push_back(std::make_shared<GameEntity>(cube, material1, Death, 100, 100, 1));
	environmentEntities[0]->GetTransform()->MoveAbsolute(0, -100, 0);

	environmentEntities.push_back(std::make_shared<GameEntity>(cube, material1, Death, 100, 100, 1));
	environmentEntities[1]->GetTransform()->MoveAbsolute(0, 100, 0);

	environmentEntities.push_back(std::make_shared<GameEntity>(cube, material1, Death, 1, 100, 100));
	environmentEntities[2]->GetTransform()->MoveAbsolute(0, 0, 100);

	environmentEntities.push_back(std::make_shared<GameEntity>(cube, material1, Death, 1, 100, 100));
	environmentEntities[3]->GetTransform()->MoveAbsolute(0, 0, -100);

	environmentEntities.push_back(std::make_shared<GameEntity>(cube, material1, Death, 100, 1, 100));
	environmentEntities[4]->GetTransform()->MoveAbsolute(-100, 0, 0);

	environmentEntities.push_back(std::make_shared<GameEntity>(cube, material1, Death, 100, 1, 100));
	environmentEntities[5]->GetTransform()->MoveAbsolute(100, 0, 0);
#pragma endregion
	//****************************

	//****************************
	// Collectibles
	//****************************
#pragma region Collectibles
	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[0]->GetTransform()->MoveAbsolute(5, 5, 5);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[1]->GetTransform()->MoveAbsolute(70, 55, -30);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[2]->GetTransform()->MoveAbsolute(23, -79, -33);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[3]->GetTransform()->MoveAbsolute(-64, -23, 42);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[4]->GetTransform()->MoveAbsolute(-57, -62, 60);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[5]->GetTransform()->MoveAbsolute(13, -84, 22);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[6]->GetTransform()->MoveAbsolute(-71, 82, 16);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[7]->GetTransform()->MoveAbsolute(51, -12, 85);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[8]->GetTransform()->MoveAbsolute(-85, 31, -77);

	collectibleEntities.push_back(std::make_shared<GameEntity>(cube, material2, Pickup, 2, 2, 2));
	collectibleEntities[9]->GetTransform()->MoveAbsolute(-33, 21, -65);
#pragma endregion
	//****************************

	// Make Skybox
	sky = std::make_shared<Sky>(FixPath(L"../../Assets/Textures/SunnyCubeMap.dds").c_str(), cube, samplerState, device, skyVertexShader, skyPixelShader);
}


// --------------------------------------------------------
// Handle resizing to match the new window size.
//  - DXCore needs to resize the back buffer
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	if (camera != 0) {
		camera->UpdateProjectionMatrix((float)windowWidth / windowHeight);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Get a reference to our custom input manager
	Input& input = Input::GetInstance();

	// Reset input manager's gui state so we don’t
	// taint our own input (you’ll uncomment later)
	input.SetKeyboardCapture(false);
	input.SetMouseCapture(false);

	// Feed fresh input data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)this->windowWidth;
	io.DisplaySize.y = (float)this->windowHeight;
	io.KeyCtrl = input.KeyDown(VK_CONTROL);
	io.KeyShift = input.KeyDown(VK_SHIFT);
	io.KeyAlt = input.KeyDown(VK_MENU);
	io.MousePos.x = (float)input.GetMouseX();
	io.MousePos.y = (float)input.GetMouseY();
	io.MouseDown[0] = input.MouseLeftDown();
	io.MouseDown[1] = input.MouseRightDown();
	io.MouseDown[2] = input.MouseMiddleDown();
	io.MouseWheel = input.GetMouseWheel();
	input.GetKeyArray(io.KeysDown, 256);

	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Determine new input capture (you’ll uncomment later)
	input.SetKeyboardCapture(io.WantCaptureKeyboard);
	input.SetMouseCapture(io.WantCaptureMouse);

	// Show the demo window
	//ImGui::ShowDemoWindow();

	//// You can create a 3 or 4-component color editor, too!
	//// Notice the two different function names below (ending with 3 and 4)
	ImGui::ColorEdit3("Edit Light 1 Color", &lights[0].Color.x);
	ImGui::DragFloat3("Edit Light 1 Direction", &lights[0].Direction.x);

	ImGui::ColorEdit3("Edit Light 2 Color", &lights[1].Color.x);
	ImGui::DragFloat3("Edit Light 2 Direction", &lights[1].Direction.x);

	ImGui::ColorEdit3("Edit Light 3 Color", &lights[2].Color.x);
	ImGui::DragFloat3("Edit Light 3 Direction", &lights[2].Direction.x);

	ImGui::ColorEdit3("Edit Light 4 Color", &lights[3].Color.x);
	ImGui::DragFloat3("Edit Light 4 Position", &lights[3].Position.x);

	ImGui::ColorEdit3("Edit Light 5 Color", &lights[4].Color.x);
	ImGui::DragFloat3("Edit Light 5 Position", &lights[4].Position.x);

	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();

	player->Update(deltaTime);

	if (timeIncreasing)
	{
		timeWindow += deltaTime;

		if (timeWindow > 1.0f)
		{
			timeIncreasing = false;
		}
	}
	else
	{
		timeWindow -= deltaTime;

		if (timeWindow < 0.0f)
		{
			timeIncreasing = true;
		}
	}

	for (std::shared_ptr<GameEntity> entity : environmentEntities)
	{
		entity->Update(deltaTime);
		player->CheckTerrainCollision(entity, entity->GetTransform());
	}

	for (std::shared_ptr<GameEntity> entity : collectibleEntities)
	{
		entity->GetTransform()->Rotate(1 * deltaTime, 3 * deltaTime, 2 * deltaTime);
		entity->Update(deltaTime);
		entity->GetTransform()->MoveAbsolute(timeIncreasing ? timeWindow : -timeWindow, 0, 0);
		player->CheckTerrainCollision(entity, entity->GetTransform());
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erases what's on the screen)
		const float bgColor[4] = { ambientColor.x, ambientColor.y, ambientColor.z, 1.0f }; // Cornflower Blue
		context->ClearRenderTargetView(backBufferRTV.Get(), bgColor);

		// Clear the depth buffer (resets per-pixel occlusion information)
		context->ClearDepthStencilView(depthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// Draw all meshes
	for (std::shared_ptr<GameEntity> entity : environmentEntities)
	{
		//Update shader
		entity->GetMaterial()->GetPixelShader()->SetFloat3("ambientColor", player->score >= 1000 ? XMFLOAT3(1, 1, 1) : ambientColor);

		entity->GetMaterial()->GetPixelShader()->SetData("lights", &lights[0], sizeof(Light) * (int)lights.size());
		
		if (entity->collider->isActive) entity->Draw(context, camera);
	}

	for (std::shared_ptr<GameEntity> entity : collectibleEntities)
	{
		//Update shader
		entity->GetMaterial()->GetPixelShader()->SetFloat3("ambientColor", ambientColor);

		entity->GetMaterial()->GetPixelShader()->SetData("lights", &lights[0], sizeof(Light) * (int)lights.size());

		if (entity->collider->isActive) entity->Draw(context, camera);
	}

	sky->Draw(context, camera);

	// Draw ImGui
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present the back buffer to the user
		//  - Puts the results of what we've drawn onto the window
		//  - Without this, the user never sees anything
		swapChain->Present(vsync ? 1 : 0, 0);

		// Must re-bind buffers after presenting, as they become unbound
		context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthBufferDSV.Get());
	}
}