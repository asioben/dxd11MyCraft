Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer cb : register(b0) {
    float4x4 VP;
}

struct Vin{
    float3 pos: POSITION;
    float2 texcoord: TEXCOORD;

   // float4x4 World: WORLD;
};

struct Vout{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

Vout VShader(Vin input){
    Vout output;

   // float4 worldPos = mul(float4(input.pos, 1.0f),input.World);

   // output.position = mul(worldPos, VP);
    output.position = mul(float4(input.pos, 1.0f),VP);
    output.texcoord = input.texcoord;

    return output;
}

float4 PShader(Vout input) : SV_TARGET{
    return tex.Sample(samp,input.texcoord);
}