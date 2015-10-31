#include "ShaderCommon.hlsli"

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

PixelShaderInput main( float4 pos : POSITION ) 
{
	PixelShaderInput PSIn;
	PSIn.pos = mul(pos,u_matrix);
	PSIn.patternAB.xy = mul(float3(pos.x, pos.y, 1), pattern_a);
	PSIn.patternAB.zw = mul(float3(pos.x, pos.y, 1), pattern_b);
	return PSIn;
}




//uniform mat4 u_matrix;
//uniform mat3 u_patternmatrix_a;
//uniform mat3 u_patternmatrix_b;
//
//attribute vec2 a_pos;
//
//varying vec2 v_pos_a;
//varying vec2 v_pos_b;
//
//void main() {
//    gl_Position = u_matrix * vec4(a_pos, 0, 1);
//    v_pos_a = (u_patternmatrix_a * vec3(a_pos, 1)).xy;
//    v_pos_b = (u_patternmatrix_b * vec3(a_pos, 1)).xy;
//}
