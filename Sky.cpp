#include "Sky.h"
#include "Helpers.h"
using namespace DirectX;



Sky::Sky(const wchar_t* texturePath, std::shared_ptr<Mesh> _mesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerState, Microsoft::WRL::ComPtr<ID3D11Device> _device, std::shared_ptr<SimpleVertexShader> _vertexShader, std::shared_ptr<SimplePixelShader> _pixelShader)
{
	mesh = _mesh;
	samplerState = _samplerState;
	device = _device;
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;

	InitializeStates();
	CreateDDSTextureFromFile(device.Get(), texturePath, 0, SRV.GetAddressOf());
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	context->RSSetState(rasterizerState.Get());
	context->OMSetDepthStencilState(depthState.Get(), 0);

	vertexShader->SetShader();
	pixelShader->SetShader();

	pixelShader->SetShaderResourceView("SkyTexture", SRV);
	pixelShader->SetSamplerState("BasicSampler", samplerState);

	vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	vertexShader->CopyAllBufferData();

	mesh->Draw(context);

	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

void Sky::InitializeStates()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&depthDesc, depthState.GetAddressOf());
}