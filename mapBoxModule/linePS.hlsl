#include "ShaderCommon.hlsli"

cbuffer lineData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	row_major matrix<float, 4, 4>  u_exmatrix;
	float  u_ratio;
	float2 u_linewidth;
	float4 u_color;
	float u_blur;
};

float4 main(PixelShaderInput psData) : SV_TARGET
{
	float dist = length(psData.patternAB.xy) * u_linewidth.x;
	float alpha = clamp(min(dist - (u_linewidth.y - u_blur), u_linewidth.x - dist) / u_blur, 0.0, 1.0);
	return u_color * alpha;
}


