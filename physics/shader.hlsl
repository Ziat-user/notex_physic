// shader.hlsl
// b0: WVP (VS)
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
};
// b2: 描画モード (PS)  0=矩形  1=円SDF
cbuffer DrawModeBuffer : register(b2)
{
    int   DrawMode;
    float Padding0;
    float Padding1;
    float Padding2;
};
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float4 Diffuse  : COLOR;
    float2 TexCoord : TEXCOORD;
};
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Diffuse  : COLOR;
    float2 TexCoord : TEXCOORD;
};
PS_INPUT VertexShaderPolygon(VS_INPUT input)
{
    PS_INPUT output;
    output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection);
    output.Diffuse  = input.Diffuse;
    output.TexCoord = input.TexCoord;
    return output;
}
float4 PixelShaderPolygon(PS_INPUT input) : SV_TARGET
{
    if (DrawMode == 1)
    {
        float2 uv    = input.TexCoord;
        float  dist  = length(uv - float2(0.5f, 0.5f));
        float  alpha = smoothstep(0.5f, 0.495f, dist);
        clip(alpha - 0.001f);
        return float4(input.Diffuse.rgb, input.Diffuse.a * alpha);
    }
    return input.Diffuse;
}
