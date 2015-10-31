#pragma once

namespace mbgl
{
	namespace dx {

		class IconDXBuffer : public DirectX::PrimitiveBatch<std::array<byte, 48>>
		{
		public:
			IconDXBuffer(_In_ ID3D11DeviceContext* deviceCtx);
			~IconDXBuffer();

			void addData(DirectX::XMFLOAT4 pos, DirectX::XMFLOAT4 adata1, DirectX::XMFLOAT4 adata2);
			size_t add(int16_t x, int16_t y, float ox, float oy, int16_t tx, int16_t ty, float angle, float minzoom, std::array<float, 2> range, float maxzoom, float labelminzoom);

		private:
			int strideIndex;

			static const double angleFactor;
		};

	}
}