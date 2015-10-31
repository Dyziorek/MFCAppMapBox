
#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);

cbuffer linesdfPSParam : register (b0)
{
	float4 u_color;
	float2 u_linewidth;
	float u_blur;
	float u_sdfgamma;
	float u_mix;
};

float4 main(PSINSDF input) : SV_TARGET
{
	
	float dist = length(input.normal) * u_linewidth.x;
	float alpha = clamp(min(dist - (u_linewidth.y - u_blur), u_linewidth.x - dist) / u_blur, 0.0, 1.0);
	float sdfdist_a = u_image.Sample(TextureSampler, input.tex_a).a;
	float sdfdist_b = u_image.Sample(TextureSampler, input.tex_b).a;
	float sdfdist = lerp(sdfdist_a, sdfdist_b, u_mix);
	alpha *= smoothstep(0.5 - u_sdfgamma, 0.5 + u_sdfgamma, sdfdist);

	return u_color * alpha;
}

//
//R"linesdf_frag(#version 120
//uniform vec2 u_linewidth;
//uniform vec4 u_color;
//uniform float u_blur;
//uniform sampler2D u_image;
//uniform float u_sdfgamma;
//uniform float u_mix;
//
//varying vec2 v_normal;
//varying vec2 v_tex_a;
//varying vec2 v_tex_b;
//
//void main() {
//    // Calculate the distance of the pixel from the line in pixels.
//    float dist = length(v_normal) * u_linewidth.s;
//
//	    // Calculate the antialiasing fade factor. This is either when fading in
//    // the line in case of an offset line (v_linewidth.t) or when fading out
//    // (v_linewidth.s)
//    float alpha = clamp(min(dist - (u_linewidth.t - u_blur), u_linewidth.s - dist) / u_blur, 0.0, 1.0);
//
//	    float sdfdist_a = texture2D(u_image, v_tex_a).a;
//    float sdfdist_b = texture2D(u_image, v_tex_b).a;
//    float sdfdist = mix(sdfdist_a, sdfdist_b, u_mix);
//    alpha *= smoothstep(0.5 - u_sdfgamma, 0.5 + u_sdfgamma, sdfdist);
//
//	    gl_FragColor = u_color * alpha;
//}
//)linesdf_frag",