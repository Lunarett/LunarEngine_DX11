 Texture2D mainTex;
SamplerState mainSampler;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

float4 main(PixelInput input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;
    
    textureColor = mainTex.Sample(mainSampler, input.uv);
    color = ambientColor;
    
    float3 normal = input.normal;
    normal = normalize(input.normal);
    
    specular = specularColor;
    
    lightDir = lightDirection;
    lightIntensity = saturate(dot(lightDir, input.normal));
    
    if (lightIntensity > 0)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        
        reflection = normalize(2 * lightIntensity * normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }
    
    color = color * textureColor;
    color = saturate(color + specular);
    
    color.a = 1;
    
//    float4 colorTest = float4(1, 0, 1, 1);
//    float4 textureColorTest = mainTex.Sample(mainSampler, input.uv);
    
    return color;
}