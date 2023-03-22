#include "Structs.hlsli"

// Define a cbuffer to use C++ data
cbuffer ExternalData : register(b0)
{
    matrix view;
    matrix projection;
}

SkyVertexToPixel main(VertexShaderInput input)
{
    SkyVertexToPixel output;

    matrix viewNoTranslation = view;
    viewNoTranslation._14 = 0;
    viewNoTranslation._24 = 0;
    viewNoTranslation._34 = 0;
    
    matrix viewProjection = mul(projection, viewNoTranslation);
    output.position = mul(viewProjection, float4(input.localPosition, 1.0f));
    output.position.z = output.position.w;
    
    output.sampleDir = input.localPosition;
    
    return output;
}