
#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);

cbuffer sdfParam : register (b0)
{
	row_major matrix<float, 4, 4> u_matrix;
	row_major matrix<float, 4, 4> u_exmatrix;
	float u_angle;
	float u_zoom;
	float u_flip;
	float u_fadedist;
	float u_minfadezoom;
	float u_maxfadezoom;
	float u_fadezoom;
	float2 u_texsize;
	float4 u_color;
	float u_buffer;
	float u_gamma;
};



float4 main(SDFPSIN psIN) : SV_TARGET
{
	float dist = u_image.Sample(TextureSampler, psIN.v_tex.xy).a;
	float alpha = smoothstep(u_buffer - u_gamma, u_buffer + u_gamma, dist) * psIN.v_tex.z;
	return u_color * alpha;
}


