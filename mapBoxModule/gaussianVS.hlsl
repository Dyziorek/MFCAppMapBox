#include "ShaderCommon.hlsli"

cbuffer rasterData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	float2 u_offset;
};

GaussPSIn main( float4 pos : POSITION )
{
	GaussPSIn PSin;
	PSin.pos = mul(u_matrix, pos);
	float2 tex = pos.xy / 2.0 + 0.5;
	PSin.v_coords[0] = tex;
	PSin.v_coords[1] = tex + u_offset * 1.1824255238063563;
	PSin.v_coords[2] = tex - u_offset * 1.1824255238063563;
	return PSin;
}

