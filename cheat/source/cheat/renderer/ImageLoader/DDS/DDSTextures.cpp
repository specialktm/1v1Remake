#include <DDSTextureLoader.h>
#include "DDSTextures.h"
#include "../../../../util/logger/logger.hpp"
namespace cheat
{
    ID3D11ShaderResourceView* DDSTextureLoader::LoadDDSFromFile(ID3D11Device* device, const fs::path& path)
    {
        ID3D11ShaderResourceView* resource_view = nullptr;
        const std::string key = path.filename().string();

        auto textureCache = GetTextureCache();
        auto it = textureCache.find(key);
        if (it != textureCache.end())
        {
            g_logger->send(levels::debug, "Texture cache found for: {}", key);
            return it->second;
        }

        g_logger->send(levels::debug, "Loading DDS texture: {}", key);

        HRESULT m_status = DirectX::CreateDDSTextureFromFile(device, path.wstring().c_str(), nullptr, &resource_view);
        if (FAILED(m_status))
        {
            g_logger->send(levels::error, "Failed to load DDS: {}", path.string());
            return nullptr;
        }

        if (resource_view)
        {
            AddToTextureCache(key, resource_view);
            g_logger->send(levels::debug, "DDS Texture cached: {}", key);
        }

        return resource_view;
    }


}