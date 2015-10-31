#include "pch.h"
#include <mbgl/platform/win32/DX.hpp>
#include "DirectXHelpers.h"
#include "DemandCreate.h"
#include "ConstantBuffer.h"
#include "Icon_DXShader.h"
#include <d3dcompiler.h>

namespace mbgl {
	namespace dx
	{
		Icon_DXShader::Icon_DXShader(_In_ ID3D11Device* device, const BYTE* shaderBytesVS, const int shaderByteSizeVS, const BYTE* shaderBytesPS, const int shaderByteSizePS)
			: DXShader(device, shaderBytesVS,shaderByteSizeVS, shaderBytesPS, shaderByteSizePS), bufferInputDesc(nullptr), bufferInputSize(0)
		{
			buffer.Create(device);
			shaderRes.DemandCreateVS(shaderVSInt, shaderVS);
			shaderRes.DemandCreatePS(shaderPSInt, shaderPS);

			D3D11_INPUT_ELEMENT_DESC IA[] =  {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL0",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL1",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			};


			bufferInputDesc = IA;
			bufferInputSize = sizeof(IA);

			HRESULT hr = device->CreateInputLayout(IA, 3, shaderVS.shader_bytes, shaderVS.shader_size, dataLayout.GetAddressOf());
			
		}


		Icon_DXShader::~Icon_DXShader()
		{
		}

		void Icon_DXShader::SetBuffer(_In_ ID3D11DeviceContext* deviceCtx,  IconBuffer& value)
		{
			buffer.SetData(deviceCtx, value);
		}


		bool Icon_DXShader::getInputElementDesc(D3D11_INPUT_ELEMENT_DESC** outIA, int* descCount)
		{
			if (bufferInputDesc != nullptr && bufferInputSize != 0)
			{
				*outIA = bufferInputDesc;
				*descCount = bufferInputSize;
				return true;
			}
			return false;
		}
	}
}