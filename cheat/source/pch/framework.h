#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include <winsdkver.h>
#include <sdkddkver.h>
#include <winternl.h>

#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <Psapi.h>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <array>
#include <map>
#include <wrl/client.h>

// DirectX
#include <d3d11.h>
#include <dxgi.h>

// Stbi
#include "stb_image.h"
#include "stb_image_write.h"

// ImGui
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

// Util
#include "../util/util.h"