#include "ShaderCommon.hlsli"

cbuffer rasterData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	float u_buffer;
	float u_opacity;
	float u_brightness_low;
	float u_brightness_high;
	float u_saturation_factor;
	float u_contrast_factor;
	float3 u_spin_weights;
};


PixelShaderInput main(float4 pos : POSITION)
{
	PixelShaderInput PSIN;
	PSIN.pos = mul(pos, u_matrix);
	float dimension = (4096.0 + 2.0 * u_buffer);
	float x_pos = (pos.x / dimension) + (u_buffer / dimension);
	float y_pos = (pos.y / dimension) + (u_buffer / dimension);
	PSIN.patternAB.xy = float2(x_pos, y_pos);
	PSIN.patternAB.zw = float2(0, 1);
	return PSIN;
}
