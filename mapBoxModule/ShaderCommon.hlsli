//constant buffers



//Pixel Shader Inputs



struct PixelShaderInput
{
	float4 pos : SV_Position;
	float4 patternAB :POSITION0;
};

struct PSInIcon
{
	float4 pos : SV_Position;
	float4 patternAB :POSITION0;
	float4 patternCD :POSITION1;
};


struct PixelShaderInputSizer
{
	float4 pos : SV_Position;
	float4 patternAB :POSITION0;
	float psize : PSIZE;
};


struct PSINSDF
{
	float4 pos : SV_Position;
	float2 normal :NORMAL;
	float2 tex_a :TEXCOORD0;
	float2 tex_b :TEXCOORD1;
};

struct GaussPSIn
{
	float4 pos : SV_Position;
	float2 v_coords[3]:NORMAL;
};


struct SDFPSIN
{
	float4 pos : SV_Position;
	float3 v_tex :NORMAL;
};

