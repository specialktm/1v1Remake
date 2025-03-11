#pragma once

#include "../ImageLoader.h"

namespace cheat
{
	class DDSTextureLoader : public ImageLoader
	{
	public:
		ID3D11ShaderResourceView* LoadDDSFromFile(ID3D11Device* device, const fs::path& path);
	};
	inline DDSTextureLoader g_DDSTextureLoader;
}