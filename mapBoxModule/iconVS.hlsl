#include "ShaderCommon.hlsli"

cbuffer iconData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	row_major matrix<float, 4, 4> u_exmatrix;

	float u_angle;
	float u_zoom;
	float u_flip;
	float u_fadedist;
	float u_minfadezoom;
	float u_maxfadezoom;
	float u_fadezoom;
	float u_opacity;

	float2 u_texsize;
};


SDFPSIN main( float4 a_pos : POSITION , float4 a_data1 : NORMAL0, float4 a_data2 : NORMAL1) 
{
	SDFPSIN psIN;
	
	float2 a_tex = a_data1.xy;
	float a_labelminzoom = a_data1.z;
	float a_angle = a_data1.y;
	float2 a_zoom = a_data2.xy;
	float a_minzoom = a_zoom.x;
	float a_maxzoom = a_zoom.y;
	float2 a_range = a_data2.zw;
	float a_rangeend = a_range.x;
	float a_rangestart = a_range.y;

	float a_fadedist = 10.0;
	float rev = 0.0;

	// u_angle is angle of the map, -128..128 representing 0..2PI
	// a_angle is angle of the label, 0..256 representing 0..2PI, where 0 is horizontal text
	float rotated = fmod(a_angle + u_angle, 256.0);
	// if the label rotates with the map, and if the rotated label is upside down, hide it
	if (u_flip > 0.0 && rotated >= 64.0 && rotated < 192.0) rev = 1.0;

	// If the label should be invisible, we move the vertex outside
	// of the view plane so that the triangle gets clipped. This makes it easier
	// for us to create degenerate triangle strips.
	// u_zoom is the current zoom level adjusted for the change in font size
	float z = 2.0 - step(a_minzoom, u_zoom) - (1.0 - step(a_maxzoom, u_zoom)) + rev;

	// fade out labels
	float alpha = clamp((u_fadezoom - a_labelminzoom) / u_fadedist, 0.0, 1.0);

	if (u_fadedist >= 0.0) {
		psIN.v_tex.z = alpha;
	}
	else {
		psIN.v_tex.z = 1.0 - alpha;
	}
	if (u_maxfadezoom < a_labelminzoom) {
		psIN.v_tex.z = 0.0;
	}
	if (u_minfadezoom >= a_labelminzoom) {
		psIN.v_tex.z = 1.0;
	}

	// if label has been faded out, clip it
	z += step(psIN.v_tex.z, 0.0);

	// all the angles are 0..256 representing 0..2PI
	// hide if (angle >= a_rangeend && angle < rangestart)
	z += step(a_rangeend, u_angle) * (1.0 - step(a_rangestart, u_angle));

	psIN.pos = mul(u_matrix ,float4(a_pos.xy, 0, 1)) + mul(u_exmatrix , float4(a_pos.zw / 64.0, z, 0));
	psIN.v_tex.xy = a_tex / u_texsize;

	psIN.v_tex.z *= u_opacity;

	return psIN;
}
