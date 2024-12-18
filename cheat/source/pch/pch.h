#pragma once
// Defines
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#define IMGUI_DEFINE_MATH_OPERATORS

// Macros
#define STRING_IMPL(x) #x
#define WSTRING_IMPL(x) L ## x
#define WIDE_IMPL(x) WSTRING_IMPL(x)
#define STR_IMPL(x) STRING_IMPL(x)


// Includes
#include "framework.h"


// Namespaces
using namespace std::chrono;
using namespace std::chrono_literals;
namespace fs = std::filesystem;
namespace imgui = ImGui;

// Vars
inline HMODULE g_Module;
inline HANDLE g_MainThread;
inline bool g_Running = true;

const std::vector<std::string> ValidExtensions = { ".png", ".jpeg", ".jpg",".bmp", ".psd", ".tga", ".gif", ".hdr", ".pic", ".ppm", ".pgm" };

// ComPtr
template <typename t>
using comPtr = Microsoft::WRL::ComPtr<t>;

