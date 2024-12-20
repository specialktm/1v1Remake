#pragma once
#include "../../../pch/pch.h"
// Mister9982/99Anvar99 (Github) Aera Image loading


namespace cheat
{
	struct intVec2
	{
		int x, y;
	};
	struct Image
	{
		unsigned char* Data;
		intVec2 v2;
	};

	struct ImageData
	{
		unsigned char* ImageBytes;
		int ImageLength;
		int Delay;
	};

	struct FrameData
	{
		int m_Delay;
		ID3D11ShaderResourceView* m_Texture;
	};

	class ImageLoader 
	{
	public:
		ImageLoader() = default;
		~ImageLoader() = default;
		ImageData WritePngToMemory(int x, int y, int comp, const void* data, int stride_bytes, const int delay);
		Image LoadImageFromMemory(const ImageData& data);
		ID3D11ShaderResourceView* CreateResourceView(ID3D11Device* device, unsigned char* img_data, const intVec2 img_size);
		std::map<int, ImageData> LoadGif(const fs::path& path);
		ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, const fs::path& file_path);
		std::map<int, FrameData> CreateGifTexture(ID3D11Device* device, const fs::path& path);
	private:
	};
	inline ImageLoader g_ImageLoader;
}
