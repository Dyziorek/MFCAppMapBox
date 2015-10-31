#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);

cbuffer Parameters : register(b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	row_major matrix<float, 3, 3> pattern_a;
	row_major matrix<float, 3, 3> pattern_b;
	float u_opacity;
	float2 u_pattern_tl_a;
	float2 u_pattern_br_a;
	float2 u_pattern_tl_b;
	float2 u_pattern_br_b;
	float u_mix;
};

float4 main(PixelShaderInput psIN) : SV_TARGET
{

	float2 imagecoord = fmod(psIN.patternAB.xy, 1.0);
	float2 pos = lerp(u_pattern_tl_a, u_pattern_br_a, imagecoord);
	float4 color1 = u_image.Sample(TextureSampler, pos);

	float2 imagecoord_b = fmod(psIN.patternAB.zw, 1.0);
	float2 pos2 = lerp(u_pattern_tl_b, u_pattern_br_b, imagecoord_b);
	float4 color2 = u_image.Sample(TextureSampler, pos2);

	
	return  mul(lerp(color1, color2, u_mix), u_opacity);
	
}






