#include "ShaderCommon.hlsli"
#define scale 0.015873016

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


PixelShaderInputSizer main( float4 a_pos : POSITION, float4 a_data : NORMAL)
{
	PixelShaderInputSizer PS;
	float2 a_extrude = a_data.xy;
	float a_linesofar = a_data.z * 128.0 + a_data.w;
	float2 normal = fmod(a_pos.xy, 2.0);
	normal.y = sign(normal.y - 0.5);
	PS.patternAB.xy = normal;
	float2 extrude = a_extrude * scale;
	float2 dist = mul(mul(u_linewidth.x , extrude) , (1.0 - u_point));
	float z = step(32767.0, a_pos.x);
	z += u_point * step(1.0, PS.patternAB.y);
	PS.pos = mul(u_matrix , float4(floor(a_pos.xy / 2.0), 0.0, 1.0)) + mul(u_exmatrix , float4(dist, z, 0.0));
	PS.patternAB.zw = a_linesofar;
	PS.psize = 2.0 * u_linewidth.x - 1.0;
	return PS;
}

