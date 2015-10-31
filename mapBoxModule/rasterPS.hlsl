#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);

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



float4 main(PixelShaderInput psIN, float2 PointCoord :VPOS) : SV_TARGET
{
	float2 posraster = PointCoord;
	float4 color = u_image.Sample(TextureSampler, posraster);
	color *= u_opacity;
	float3 rgb = color.rgb;
	rgb = float3(
		dot(rgb, u_spin_weights.xyz),
		dot(rgb, u_spin_weights.zxy),
		dot(rgb, u_spin_weights.yzx));
	float average = (color.r + color.g + color.b) / 3.0;
	rgb += (average - rgb) * u_saturation_factor;

	rgb = (rgb - 0.5) * u_contrast_factor + 0.5;

	float3 u_high_vec = float3(u_brightness_low, u_brightness_low, u_brightness_low);
	float3 u_low_vec = float3(u_brightness_high, u_brightness_high, u_brightness_high);

	return float4(lerp(u_high_vec, u_low_vec, rgb), color.a);
}


