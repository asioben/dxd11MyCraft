cbuffer cb: register(b0){
    float4x4 WVP;
}

struct Vout{
    float4 position: SV_POSITION;
    float4 color: COLOR;
};

Vout VShader(float3 position: POSITION, float4 color: COLOR){
    Vout output;

    output.position = mul(float4(position,1.0f),WVP);
    output.color = color;

    return output;
}

float4 PShader(float4 color: COLOR) : SV_TARGET{
    return color;
}
