#pragma once

namespace mbgl
{
	namespace dx {

template <size_t sizeElem, int16_t maxSize = 8192, int16_t bindFlags = D3D11_BIND_VERTEX_BUFFER>
class DXBuffers
{
public:
	DXBuffers(_In_ ID3D11DeviceContext* deviceCtx) {
		strideSize = sizeElem;
		maxVertices = maxSize;

		Microsoft::WRL::ComPtr<ID3D11Device> devicePtr(deviceCtx->GetDevice());

		CreateBuffer(devicePtr, strideSize * maxVertices, bindFlags, vertexBuffer.GetAddressOf());
		}
	~DXBuffers() {}


	static void CreateBuffer(_In_ ID3D11Device* device, size_t bufferSize, D3D11_BIND_FLAG bindFlag, _Out_ ID3D11Buffer** pBuffer)
	{
		D3D11_BUFFER_DESC desc = { 0 };

		desc.ByteWidth = (UINT)bufferSize;
		desc.BindFlags = bindFlag;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		DirectX::ThrowIfFailed(
			device->CreateBuffer(&desc, nullptr, pBuffer)
			);

		
	}


	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	size_t strideSize;
	uint16_t maxVertices;
	uint16_t bindFlag;
};




	}
}
