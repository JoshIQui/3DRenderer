#pragma once
#include "DXCore.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <fstream>
#include <DirectXMath.h>
#include <vector>
#include "AABB.h"

class Mesh
{
public:
	Mesh(Vertex vertices[], size_t vertexCount, unsigned int indices[], size_t indexCount, Microsoft::WRL::ComPtr<ID3D11Device> device);
	Mesh(const std::wstring& objFile, Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Mesh();

	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();

	void CreateMeshBuffers(Vertex vertices[], size_t vertexCount, unsigned int indices[], size_t indexCount, Microsoft::WRL::ComPtr<ID3D11Device> device);

	std::vector<Vertex> verts;
	int numVerts;

	unsigned int GetIndexCount();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	unsigned int indexCount;
};

