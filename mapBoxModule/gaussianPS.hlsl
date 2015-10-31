#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);

float4 main(GaussPSIn gaussIN) : SV_TARGET
{
	float4 sum = float4(0.0, 0.0,0.0,0.0);
	sum += u_image.Sample(TextureSampler, gaussIN.v_coords[0]) * 0.40261994689424746;
	sum += u_image.Sample(TextureSampler, gaussIN.v_coords[1]) * 0.2986900265528763;
	sum += u_image.Sample(TextureSampler, gaussIN.v_coords[2]) * 0.2986900265528763;
	return sum;
}