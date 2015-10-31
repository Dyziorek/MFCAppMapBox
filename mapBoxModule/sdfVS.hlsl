#include "ShaderCommon.hlsli"

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


SDFPSIN main( float4 pos : POSITION, float4 data1 : NORMAL0, float4 data2 : NORMAL1) 
{
	SDFPSIN sdfPS;
	float2 a_tex = data1.xy;
	float a_labelminzoom = data1.z;
	float a_angle = data1.w;
	float2 a_zoom = data2.xy;
	float a_minzoom = a_zoom.x;
	float a_maxzoom = a_zoom.y;
	float2 a_range = data2.zw;
	float a_rangeend = a_range[0];
	float a_rangestart = a_range[1];
	float rev = 0.0;
	float rotated = fmod(a_angle + u_angle, 256.0);
	// if the label rotates with the map, and if the rotated label is upside down, hide it
	if (u_flip > 0.0 && rotated >= 64.0 && rotated < 192.0) rev = 1.0;

	float z = 2.0 - step(a_minzoom, u_zoom) - (1.0 - step(a_maxzoom, u_zoom)) + rev;

	float alpha = clamp((u_fadezoom - a_labelminzoom) / u_fadedist, 0.0, 1.0);
	if (u_fadedist >= 0.0) {
		sdfPS.v_tex.z = alpha;
	}
	else {
		sdfPS.v_tex.z = 1.0 - alpha;
	}
	if (u_maxfadezoom < a_labelminzoom) {
		sdfPS.v_tex.z = 0.0;
	}
	if (u_minfadezoom >= a_labelminzoom) {
		sdfPS.v_tex.z = 1.0;
	}

	// if label has been faded out, clip it
	z += step(sdfPS.v_tex.z, 0.0);

	// all the angles are 0..256 representing 0..2PI
	// hide if (angle >= a_rangeend && angle < rangestart)
	z += step(a_rangeend, u_angle) * (1.0 - step(a_rangestart, u_angle));

	sdfPS.pos = mul(u_matrix , float4(pos.xy, 0, 1)) + mul(u_exmatrix,float4(pos.zw / 64.0, z, 0));
	sdfPS.v_tex.xy = a_tex / u_texsize;

	return sdfPS;
}


