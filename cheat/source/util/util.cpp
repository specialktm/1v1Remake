#include "../pch/pch.h"
#include "util.h"
#include "../cheat/renderer/renderer.hpp"

namespace cheat::util
{
   


    uint8_t char_to_byte(char c)
    {
        constexpr uint8_t offset_0 = '0';
        constexpr uint8_t offset_a = 'a' - 10;
        constexpr uint8_t offset_A = 'A' - 10;

        c |= 0x20; // Convert uppercase to lowercase

        if (c >= '0' && c <= '9') return static_cast<uint8_t>(c - offset_0);
        if (c >= 'a' && c <= 'f') return static_cast<uint8_t>(c - offset_a);
        return 0;
    }

    std::optional<std::uint8_t> char_to_hex(const char c)
    {
        if (c >= 'a' && c <= 'f')
            return static_cast<std::uint8_t>(static_cast<std::uint32_t>(c) - 87);
        if (c >= 'A' && c <= 'F')
            return static_cast<std::uint8_t>(static_cast<std::uint32_t>(c) - 55);
        if (c >= '0' && c <= '9')
            return static_cast<std::uint8_t>(static_cast<std::uint32_t>(c) - 48);
        return {};
    }

    uint32_t joaat(const std::string_view& text)
    {
        uint32_t hash = 0;
        for (const char c : text)
        {
            hash += c;
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        return hash;
    }

    std::string convert_to_title_case(const std::string& text)
    {
        std::string result = text;
        bool capitalize_next = true;
        for (char& c : result)
        {
            if (std::isalpha(c))
            {
                c = capitalize_next ? std::toupper(c) : std::tolower(c);
                capitalize_next = false;
            }
            else
            {
                capitalize_next = true;
            }
        }
        return result;
    }

    std::string convert_to_small_case(const std::string& text)
    {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::string convert_to_caps(const std::string& text)
    {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    bool WorldToScreen(Unity::Vector3 world, Vector2& screen)
    {
        Unity::CCamera* CameraMain = Unity::Camera::GetMain(); // Get The Main Camera
        if (!CameraMain) {
         //   printf_s("false\n");
            return false;
        }

        //Unity::Vector3 buffer = WorldToScreenPoint(CameraMain, world, 2);

        Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>("WorldToScreenPoint", world, Unity::eye::mono); // Call the worldtoscren function using monoeye (2)

        if (buffer.x > g_Renderer->ScreenSize.x || buffer.y > g_Renderer->ScreenSize.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
        {
           // printf("Buffer: (%f, %f, %f), Screen Size: (%f, %f)\n",
              //  buffer.x, buffer.y, buffer.z, g_Renderer->ScreenSize.x, g_Renderer->ScreenSize.y);

            //printf_s("false 2\n");
            return false;
        }

        if (buffer.z > 0.0f) // Check if point is in view
        {
           // printf_s("is view\n");
            screen = Vector2(buffer.x, g_Renderer.get()->ScreenSize.y - buffer.y);
        }

        if (screen.x > 0 || screen.y > 0) // Check if point is in view
        {
            //printf_s("true\n");
            return true;
        }
    }
}