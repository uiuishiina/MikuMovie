

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
cbuffer cbuff0 : register(b0)
{
    matrix mat;
};
struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};
float4 PS(Output input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.uv));
}