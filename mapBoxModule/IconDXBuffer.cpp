#include "pch.h"
#include <mbgl/util/math.hpp>
#include <mbgl/platform/win32/DX.hpp>
#include "DirectXHelpers.h"
#include "DemandCreate.h"
#include "DXBuffers.hpp"
#include "PrimitiveBatch.h"

#include "IconDXBuffer.hpp"

namespace mbgl
{
	namespace dx {

		const double IconDXBuffer::angleFactor = 128.0 / M_PI;

		IconDXBuffer::IconDXBuffer(_In_ ID3D11DeviceContext* deviceCtx) : PrimitiveBatch(deviceCtx)
		{
			strideIndex = 0;
		}


		IconDXBuffer::~IconDXBuffer()
		{
		}

		void IconDXBuffer::addData(DirectX::XMFLOAT4 pos, DirectX::XMFLOAT4 adata1, DirectX::XMFLOAT4 adata2)
		{
			std::array<byte, 48> drawElem;
			DirectX::XMFLOAT4* posElem = static_cast<DirectX::XMFLOAT4*>((void*)&drawElem);
			memcpy_s(posElem, sizeof(DirectX::XMFLOAT4), &pos, sizeof(DirectX::XMFLOAT4));
			memcpy_s(posElem + 1, sizeof(DirectX::XMFLOAT4), &adata1, sizeof(DirectX::XMFLOAT4));
			memcpy_s(posElem + 2, sizeof(DirectX::XMFLOAT4), &adata2, sizeof(DirectX::XMFLOAT4));

			Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &drawElem, 1);

		}

		size_t IconDXBuffer::add(int16_t x, int16_t y, float ox, float oy, int16_t tx, int16_t ty, float angle, float minzoom, std::array<float, 2> range, float maxzoom, float labelminzoom) {

			DirectX::XMFLOAT4 a_pos;
			DirectX::XMFLOAT4 a_data1;
			DirectX::XMFLOAT4 a_data2;

			a_pos.x = x;
			a_pos.y = y;
			a_pos.z = std::round(ox * 64);
			a_pos.w = std::round(oy * 64);
			//shorts[0] /* pos */ = x;
			//shorts[1] /* pos */ = y;
			//shorts[2] /* offset */ = std::round(ox * 64); // use 1/64 pixels for placement
			//shorts[3] /* offset */ = std::round(oy * 64);


			//uint8_t *ubytes = static_cast<uint8_t *>(data);
			// a_data1
			a_data1.x /* tex */ = tx / 4;
			a_data1.y /* tex */ = ty / 4;
			a_data1.z /* labelminzoom */ = labelminzoom * 10;
			a_data1.w /* angle */ = (int16_t)std::round(angle * angleFactor) % 256;

			// a_data2
			a_data2.x /* minzoom */ = minzoom * 10; // 1/10 zoom levels: z16 == 160.
			a_data2.y /* maxzoom */ = std::fmin(maxzoom, 25) * 10; // 1/10 zoom levels: z16 == 160.
			a_data2.z /* rangeend */ = util::max((int16_t)std::round(range[0] * angleFactor), (int16_t)0) % 256;
			a_data2.w /* rangestart */ = util::min((int16_t)std::round(range[1] * angleFactor), (int16_t)255) % 256;

			addData(a_pos, a_data1, a_data2);
			return strideIndex++;
		}
	}
}