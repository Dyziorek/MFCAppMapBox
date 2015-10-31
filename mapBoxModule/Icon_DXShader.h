#pragma once
#include "D:\LocalRepos\mpbox\mapbox-gl-native-master\platform\win32\DXShader.hpp"
#include "pch.h"
#include "AlignedNew.h"

namespace mbgl {
	namespace dx
	{
		class IconResources : public ShaderResources
		{

			
		};

		__declspec(align(16)) struct IconBuffer  : DirectX::AlignedNew<IconBuffer>
		{
			DirectX::XMMATRIX u_matrix;
			DirectX::XMMATRIX u_exmatrix;
			float u_angle;
			float u_zoom;
			float u_flip;
			float u_fadedist;
			float u_minfadezoom;
			float u_maxfadezoom;
			float u_fadezoom;
			float u_opacity;
			DirectX::XMFLOAT2A u_texsize;
		};

		class Icon_DXShader :
			public DXShader
		{
		public:
			Icon_DXShader(_In_ ID3D11Device* device, const BYTE* shaderBytesVS, const int shaderByteSizeVS, const BYTE* shaderBytesPS, const int shaderByteSizePS);
			~Icon_DXShader();

			void SetBuffer(_In_ ID3D11DeviceContext* deviceCtx, IconBuffer& value);
			bool getInputElementDesc(D3D11_INPUT_ELEMENT_DESC** outIA, int* descCount);

		private:
			D3D11_INPUT_ELEMENT_DESC* bufferInputDesc;
			int bufferInputSize;
		};


		DirectX::ConstantBuffer<IconBuffer> buffer;
	}
}