#include "../pch/pch.h"
#include "util.h"
#include "../cheat/renderer/renderer.hpp"

namespace cheat::util
{

    void MouseMove(float tarx, float tary, float X, float Y, int smooth)
    {
        float ScreenCenterX = (X / 2);
        float ScreenCenterY = (Y / 2);
        float TargetX = 0;
        float TargetY = 0;

        smooth = smooth + 3;

        if (tarx != 0)
        {
            if (tarx > ScreenCenterX)
            {
                TargetX = -(ScreenCenterX - tarx);
                TargetX /= smooth;
                if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
            }

            if (tarx < ScreenCenterX)
            {
                TargetX = tarx - ScreenCenterX;
                TargetX /= smooth;
                if (TargetX + ScreenCenterX < 0) TargetX = 0;
            }
        }

        if (tary != 0)
        {
            if (tary > ScreenCenterY)
            {
                TargetY = -(ScreenCenterY - tary);
                TargetY /= smooth;
                if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
            }

            if (tary < ScreenCenterY)
            {
                TargetY = tary - ScreenCenterY;
                TargetY /= smooth;
                if (TargetY + ScreenCenterY < 0) TargetY = 0;
            }
        }
        mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);
    }
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
        Unity::CCamera* CameraMain = Unity::Camera::GetMain();
        if (!CameraMain) {
            return false;
        }

        Unity::Vector3 buffer = CameraMain->CallMethodSafe<Unity::Vector3>("WorldToScreenPoint", world, Unity::eye::mono); // Call the worldtoscren function using monoeye (2)

        if (buffer.x > g_Renderer->ScreenSize.x || buffer.y > g_Renderer->ScreenSize.y || buffer.x < 0 || buffer.y < 0 || buffer.z < 0) // check if point is on screen
        {
            return false;
        }

        if (buffer.z > 0.0f)
        {
            screen = Vector2(buffer.x, g_Renderer.get()->ScreenSize.y - buffer.y);
        }

        if (screen.x > 0 || screen.y > 0)
        {
            return true;
        }
    }
}

System_String_o* cheat::util::allocateSystemString(const wchar_t* utf16_chars, int length)
{
    if (length < 0) return nullptr;

    size_t structSize = sizeof(System_String_o) + (length - 1) * sizeof(wchar_t);
    System_String_o* sysStr = (System_String_o*)malloc(structSize);
    if (!sysStr) return nullptr;

    sysStr->fields._stringLength = length;
    memcpy(&sysStr->fields._firstChar, utf16_chars, length * sizeof(wchar_t));

    return sysStr;
}

System_String_o* cheat::util::ToSystemString(const std::string& utf8Str)
{
    if (utf8Str.empty()) return nullptr;

    int utf16_size = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (utf16_size <= 0) return nullptr;

    std::vector<wchar_t> utf16_string(utf16_size);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, utf16_string.data(), utf16_size);

    return allocateSystemString(utf16_string.data(), utf16_size - 1);
}


std::string cheat::util::SystemString(System_String_o* str)
{
    if (!str || str->fields._stringLength <= 0) return "<empty>";

    const uint16_t* utf16_chars = &str->fields._firstChar;
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)utf16_chars, str->fields._stringLength, nullptr, 0, nullptr, nullptr);

    if (utf8_size <= 0) return "<conversion error>";

    std::string utf8_string(utf8_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)utf16_chars, str->fields._stringLength, &utf8_string[0], utf8_size, nullptr, nullptr);

    return utf8_string;
}