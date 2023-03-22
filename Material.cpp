#include "Material.h"

Material::Material(std::shared_ptr<SimpleVertexShader> _vertexShader, std::shared_ptr<SimplePixelShader> _pixelShader, DirectX::XMFLOAT3 _colorTint, float _roughness)
{
    vertexShader = _vertexShader;
    pixelShader = _pixelShader;

    colorTint = _colorTint;
    roughness = _roughness;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader() { return vertexShader; }

std::shared_ptr<SimplePixelShader> Material::GetPixelShader() { return pixelShader; }

DirectX::XMFLOAT3 Material::GetColorTint() { return colorTint; }

void Material::SetColorTint(DirectX::XMFLOAT3 _colorTint) { colorTint = _colorTint; }

float Material::GetRoughness() { return roughness; }

void Material::SetRoughness(float _roughness) { roughness = _roughness; }

void Material::AddShaderResourceView(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView) { shaderResourceViews.insert({ name, shaderResourceView }); }

void Material::AddSamplerState(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState) { samplerStates.insert({ name, samplerState }); }

void Material::PrepareMaterial(Transform* transform, std::shared_ptr<Camera> camera)
{
	// Set shaders
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Vertex Shader
	std::shared_ptr<SimpleVertexShader> vsData = vertexShader; // Strings here MUST
	vsData->SetMatrix4x4("world", transform->GetWorldMatrix()); // match variable
	vsData->SetMatrix4x4("view", camera->GetViewMatrix()); // names in the
	vsData->SetMatrix4x4("projection", camera->GetProjectionMatrix()); // shader’s cbuffer!
	vsData->SetMatrix4x4("worldInvTranspose", transform->GetWorldMatrix());
	vsData->CopyAllBufferData();

	// Pixel Shader
	std::shared_ptr<SimplePixelShader> psData = pixelShader;
	psData->SetFloat3("colorTint", colorTint);
	psData->SetFloat("roughness", roughness);
	psData->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
	psData->CopyAllBufferData();

	// Bind Shader Resource Views and Sampler States
	for (auto& SRV : shaderResourceViews) { psData->SetShaderResourceView(SRV.first.c_str(), SRV.second); }
	for (auto& SS : samplerStates) { psData->SetSamplerState(SS.first.c_str(), SS.second); }

}
