#include "ShaderCommon.hlsli"

cbuffer PatternMatrixes : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	row_major matrix<float, 3, 3> pattern_a;
	row_major matrix<float, 3, 3> pattern_b;
};





PixelShaderInput main( float4 pos : POSITION ) 
{
	PixelShaderInput PSIn;
	PSIn.pos = mul(pos,u_matrix);
	PSIn.patternAB.xy = mul(float3(pos.x, pos.y, 1), pattern_a);
	PSIn.patternAB.zw = mul(float3(pos.x, pos.y, 1), pattern_b);
	return PSIn;
}


