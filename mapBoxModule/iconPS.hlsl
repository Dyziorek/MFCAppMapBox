
#include "ShaderCommon.hlsli"

Texture2D u_image : register(t0);
sampler TextureSampler : register(s0);



float4 main(SDFPSIN psIN) : SV_TARGET
{
	return  u_image.Sample(TextureSampler, psIN.v_tex.xy) * psIN.v_tex.z;
}
