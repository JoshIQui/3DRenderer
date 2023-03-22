#pragma once
#include <DirectXMath.h>
#include "SimpleShader.h"
#include <memory>
#include "Transform.h"
#include "Camera.h"
#include <unordered_map>

class Material
{
public:
	Material(std::shared_ptr<SimpleVertexShader> _vertexShader, std::shared_ptr<SimplePixelShader> _pixelShader, DirectX::XMFLOAT3 _colorTint, float _roughness);

	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();

	DirectX::XMFLOAT3 GetColorTint();
	void SetColorTint(DirectX::XMFLOAT3 _colorTint);

	float GetRoughness();
	void SetRoughness(float _roughness);

	void AddShaderResourceView(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView);
	void AddSamplerState(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState);

	void PrepareMaterial(Transform* transform, std::shared_ptr<Camera> camera);

private:
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;
	DirectX::XMFLOAT3 colorTint;
	float roughness;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> shaderResourceViews;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplerStates;
};

