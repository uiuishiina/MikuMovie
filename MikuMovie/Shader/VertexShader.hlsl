

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};
//’¸“_ˆÊ’u
Output VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output output;
    output.pos = pos;
    output.uv = uv;
    return output;
}