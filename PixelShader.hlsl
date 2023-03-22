#include "Structs.hlsli"
#include "Lighting.hlsli"

#define NUM_LIGHTS 5

Texture2D SurfaceTexture : register(t0); // "t" registers for textures
SamplerState BasicSampler : register(s0); // "s" registers for samplers
Texture2D NormalMap : register(t1);

// Define a cbuffer to use C++ data
cbuffer ExternalData : register(b0)
{
	float roughness;
	float3 colorTint;
	float3 cameraPosition;
	float3 ambientColor;
	
    Light lights[NUM_LIGHTS];
    
    float uvOffset;
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
	input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);

    input.normal = MapNormals(NormalMap, BasicSampler, input.uv, input.normal, input.tangent);

    float3 surfaceColor = SurfaceTexture.Sample(BasicSampler, input.uv + uvOffset).rgb * colorTint;

	float3 resultingLight = ambientColor * surfaceColor;

    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        Light light = lights[i];

        switch (lights[i].Type)
        {
            case LIGHT_TYPE_DIRECTIONAL:
                resultingLight += CalculateDirectionalLight(light, input.normal, surfaceColor, roughness, input.worldPos, cameraPosition);
                break;

            case LIGHT_TYPE_POINT:
                resultingLight += CalculatePointLight(light, input.normal, surfaceColor, roughness, input.worldPos, cameraPosition);
                break;
            
            case LIGHT_TYPE_SPOT:
				//totalLight += SpotLight(light, input.normal, input.worldPos, cameraPosition, roughness, colorTint);
                break;
        }
    }

	return float4(resultingLight, 1);
}