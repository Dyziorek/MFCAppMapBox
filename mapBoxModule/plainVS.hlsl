#include "ShaderCommon.hlsli"

cbuffer rasterData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	float4 u_color;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return mul(u_matrix, float4(pos.xy, 0, 1));
}

