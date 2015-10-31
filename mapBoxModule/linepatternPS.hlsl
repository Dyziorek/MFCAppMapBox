#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);

cbuffer rasterData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	row_major matrix<float, 4, 4>  u_exmatrix;
	float u_ratio;
	float2 u_linewidth;
	float4 u_color;
	float u_point;
	float u_blur;
	float2 u_pattern_size_a;
	float2 u_pattern_tl_a;
	float2 u_pattern_br_a;
	float2 u_pattern_size_b;
	float2 u_pattern_tl_b;
	float2 u_pattern_br_b;
	float u_fade;
	float u_opacity;
};



float4 main(PixelShaderInputSizer fromVS, float2 PointCoord :VPOS) : SV_TARGET
{
	
	float dist = length(fromVS.patternAB.xy) * (1.0 - u_point) + u_point * length(PointCoord * 2.0 - 1.0);
	dist *= u_linewidth.x;
	float alpha = clamp(min(dist - (u_linewidth.y - u_blur), u_linewidth.x - dist) / u_blur, 0.0, 1.0);
	float x_a = fmod(fromVS.patternAB.z / u_pattern_size_a.x, 1.0);
	float y_a = 0.5 + (fromVS.patternAB.y * u_linewidth.x / u_pattern_size_a.y);
	float2 pos_a = lerp(u_pattern_tl_a, u_pattern_br_a, float2(x_a, y_a));
	float x_b = fmod(fromVS.patternAB.z / u_pattern_size_b.x, 1.0);
	float y_b = 0.5 + (fromVS.patternAB.y * u_linewidth.x / u_pattern_size_b.y);
	float2 pos_b = lerp(u_pattern_tl_b, u_pattern_br_b, float2(x_b, y_b));

	float4 color = lerp(u_image.Sample(TextureSampler, pos_a), u_image.Sample(TextureSampler, pos_b), u_fade);
	alpha *= u_opacity;
	return color * alpha;;
}



