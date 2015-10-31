#include "ShaderCommon.hlsli"

cbuffer rasterData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	float2 u_world;
};

PixelShaderInput main( float4 pos : POSITION )
{
	PixelShaderInput PSIN;
	PSIN.pos = mul(u_matrix, float4(pos.xy, 0, 1));
	PSIN.patternAB.xy = mul(PSIN.pos.xy + 1.0 / 20, u_world);
	PSIN.patternAB.zw = float2(0, 0);
	return PSIN;
}


