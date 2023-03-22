#include "Structs.hlsli"
#include "Lighting.hlsli"

// Define a cbuffer to use C++ data
cbuffer ExternalData : register(b0)
{
	float3 colorTint;
	float time;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float x = (input.screenPosition / 1000) * time;
	float y = (input.screenPosition / 1000) * time;

	return float4(x, y, 0.5f, 1);
}