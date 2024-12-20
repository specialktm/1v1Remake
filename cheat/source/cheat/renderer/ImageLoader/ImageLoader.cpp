#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageLoader.h"
#include "../../../common/common.h"

namespace cheat
{
	ImageData ImageLoader::WritePngToMemory(int x, int y, int comp, const void* data, int stride_bytes, const int delay)
	{
		int length;
		unsigned char* png = stbi_write_png_to_mem(static_cast<const unsigned char*>(data), stride_bytes, x, y, comp,
			&length);
		if (!png)
			return {};
		return { png, length, delay };
	}

	Image ImageLoader::LoadImageFromMemory(const ImageData& data)
	{
		int width, height;
		const auto image = stbi_load_from_memory(data.ImageBytes, data.ImageLength, &width, &height, nullptr, 0);
		if (image == nullptr)
		{
			return {};
		}
		return { image, {static_cast<int>(std::round(width)), static_cast<int>(std::round(height))} };
	}

	ID3D11ShaderResourceView* ImageLoader::CreateResourceView(ID3D11Device* device, unsigned char* img_data, const intVec2 img_size)
	{
		if (!img_data) return nullptr;
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = img_size.x;
		desc.Height = img_size.y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		ID3D11Texture2D* p_texture = nullptr;
		const D3D11_SUBRESOURCE_DATA sub_resource = { img_data, desc.Width * 4, 0 };
		device->CreateTexture2D(&desc, &sub_resource, &p_texture);
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = desc.MipLevels;
		ID3D11ShaderResourceView* tmp_res = nullptr;
		device->CreateShaderResourceView(p_texture, &srv_desc, &tmp_res);
		p_texture->Release();
		return tmp_res;
	}

	std::map<int, ImageData> ImageLoader::LoadGif(const fs::path& path)
	{
		std::map<int, ImageData> frames;
		std::ifstream file(path, std::ios::binary);
		if (!file)
		{
			return frames;
		}
		file.seekg(0, std::ios::end);
		const std::streampos size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);
		if (!file.read(buffer.data(), size))
		{
			return frames;
		}
		int* delays = nullptr;
		int width, height, frame_count, comp;
		const auto data = stbi_load_gif_from_memory(reinterpret_cast<stbi_uc*>(buffer.data()), (int)size, &delays, &width,
			&height, &frame_count, &comp, 0);
		if (!data)
		{
			return frames;
		}
		const size_t bytes = static_cast<size_t>(width) * comp;
		for (auto i = 0; i < frame_count; i++)
		{
			frames.insert({ i, WritePngToMemory(width, height, comp, data + bytes * height * i, 0, delays[i]) });
		}
		stbi_image_free(data);
		return frames;
	}

	ID3D11ShaderResourceView* ImageLoader::CreateTexture(ID3D11Device* device, const fs::path& file_path)
	{
		intVec2 v2{};
		unsigned char* image_data = stbi_load(file_path.string().c_str(), &v2.x, &v2.y, nullptr, 4);
		if (image_data == nullptr)
		{
			return nullptr;
		}
		return CreateResourceView(device, image_data, v2);
	}

	std::map<int, FrameData> ImageLoader::CreateGifTexture(ID3D11Device* device, const fs::path& path)
	{
		const auto gif_data = LoadGif(path);
		std::map<int, FrameData> tmp_arr;
		for (const auto& [fst, snd] : gif_data)
		{
			const auto [data, v2] = LoadImageFromMemory(snd);
			tmp_arr.try_emplace(fst, snd.Delay, CreateResourceView(device, data, v2));
		}
#ifdef Developer
		g_logger->send(levels::debug,"Frame Count: {}", tmp_arr.size());
#endif
		return tmp_arr;
	}
}