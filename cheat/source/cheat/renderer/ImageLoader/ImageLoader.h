#pragma once
#include "../../../pch/pch.h"

namespace cheat
{
	// simply Vector2.
	struct intVec2
	{
		int x, y;
	};
	// Image: static images.
	struct Image
	{
		unsigned char* Data;
		intVec2 v2;
	};
	// Image data for memory.
	struct ImageData
	{
		unsigned char* ImageBytes;
		int ImageLength;
		int Delay;
	};
	// Frame data for gifs.
	struct FrameData
	{
		int m_Delay;
		ID3D11ShaderResourceView* m_Texture;
	};
	enum class Cache
	{
		GIF = 0,
		IMG
	};
	class ImageLoader 
	{
	public:
		ImageLoader() = default; // Constructor.
		~ImageLoader(); // Destructor.
		
		// Write the image data to memory.
		ImageData WritePngToMemory(int x, int y, int comp, const void* data, int stride_bytes, const int delay);
		// Loads image from memory with image data.
		Image LoadImageFromMemory(const ImageData& data);
		// Create D3D11 Resource for the texture loading.
		ID3D11ShaderResourceView* CreateResourceView(ID3D11Device* device, unsigned char* img_data, const intVec2 img_size);
		// Load the Image Data for the gif in the path.
		std::map<int, ImageData> LoadGif(const fs::path& path);
		// Create the texture and cache it.
		ID3D11ShaderResourceView* CreateTexture(ID3D11Device* device, const fs::path& file_path);
		// Create the gif and cache it.
		std::map<int, FrameData> CreateGifTexture(ID3D11Device* device, const fs::path& path);
		// Clear Gif/Texture Cache.
		void ClearCache(); 
		// Get Cache
		template<typename ReturnType>
		ReturnType GetCache(Cache CacheType);
		
		bool IsCached(const std::string_view& key);
	private:
		// Cached Gifs.
		std::unordered_map<std::string, std::map<int, FrameData>> GifCache_;
		// Cached Textures.
		std::unordered_map<std::string, ID3D11ShaderResourceView*> TextureCache_;

	};
	inline ImageLoader g_ImageLoader;
}
