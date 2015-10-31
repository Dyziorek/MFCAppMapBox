#include "ShaderCommon.hlsli"

cbuffer outlinePSParam : register (b0)
{
	float4 u_color;
};

float4 main(PixelShaderInput PSIN, float2 PointCoord :VPOS) : SV_TARGET
{
	float dist = length(PSIN.patternAB.xy - PointCoord.xy);
	float alpha = smoothstep(1.0, 0.0, dist);
	return float4(u_color * alpha);
}
