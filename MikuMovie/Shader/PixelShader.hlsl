

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 PS(Output input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.uv));
}