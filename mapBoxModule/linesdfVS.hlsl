#include "ShaderCommon.hlsli"

#define scale 0.015873016

cbuffer linesdfMatrixes : register (b0)
{
	row_major matrix<float, 4, 4> u_matrix;
	row_major matrix<float, 4, 4> u_exmtrix;
	float u_ratio;
	float2 u_linewidth;
	float2 u_patternscale_a;
	float u_tex_y_a;
	float2 u_patternscale_b;
	float u_tex_y_b;

};


PSINSDF main( float4 pos : POSITION, float4 data : NORMAL ) 
{
	PSINSDF psIN;
	float2 vExtr = data.xy;
	float a_linesofar = data.z * 128.0 + data.w;
	float2 normal = fmod(pos, 2.0).xy;
	normal.y = sign(normal.y - 0.5);
	psIN.normal = normal;
	float2 vecDist = mul(u_linewidth.x, vExtr) * scale;
	float4 dist = float4(vecDist.x, vecDist.y, 0.0, 0.0);
	float2 posXY = floor(pos * 0.5).xy;
	psIN.pos = mul(u_matrix, float4(posXY.x, posXY.y, 0.0, 0.0));
	psIN.tex_a = float2(a_linesofar * u_patternscale_a.x, normal.y * u_patternscale_a.y + u_tex_y_a);
	psIN.tex_b = float2(a_linesofar * u_patternscale_b.x, normal.y * u_patternscale_b.y + u_tex_y_b);
	return psIN;
}


