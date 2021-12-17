cbuffer matrixBuffer
{
    matrix worldViewProjectionMatrix;
    matrix worldMatrix;
};

cbuffer cameraBuffer
{
    float3 cameraPosition;
    float pad;
};

struct VertexInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOOD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXTCOORD1;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    float4 worldPosition = float4(0, 0, 0, 0);
    
    float4 newPos = float4(input.position.xyz, 1.0f);
    newPos.w = 1.0f;
    
    output.position = mul(worldViewProjectionMatrix, newPos);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    
    //worldPosition = mul(newPos, worldMatrix);
    output.viewDirection = cameraPosition.xyz - input.position;
    output.viewDirection = normalize(output.viewDirection);
    
    return output;
}