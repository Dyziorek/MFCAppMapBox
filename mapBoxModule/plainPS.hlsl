
cbuffer rasterData : register (b0)
{
	row_major matrix<float, 4, 4>  u_matrix;
	float4 u_color;
};

float4 main() : SV_TARGET
{
	return u_color;
}