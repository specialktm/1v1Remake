#pragma once
/*
* File Name: detour.h
* Purpose: Contains the code for the easy detour.
* Author VoDKa
* Date: 12.07.2024 | EU Format
* Time: 22:53
*/

// includes
#include "../../../pch/pch.h"
#include "../../../util/logger/logger.hpp"
#include <MinHook.h>

// ***
namespace cheat
{
    class DetourManager 
    {
        struct Template
        {
            std::string display_name;
            LPVOID detour_address;
        };
    public:
       inline static int total_hooks = 0;
        template<typename T>
        static MH_STATUS detour(const std::string& display, LPVOID address, T** og_function, T* detour_function) noexcept
        {
            if (address == nullptr)
            {
                g_logger->send(levels::critical, "nullptr address: {}", display);
                return MH_UNKNOWN;
            }
            total_hooks++;

            DWORD oldProtect;
            if (!VirtualProtect(address, sizeof(LPVOID), PAGE_EXECUTE_READWRITE, &oldProtect))
            {
                auto error = GetLastError();
                g_logger->send(levels::error, "Failed to change memory protection for: {} Error: {}", display, error);
                return MH_ERROR_MEMORY_PROTECT;
            }

            MH_STATUS status = MH_CreateHook(address, reinterpret_cast<LPVOID>(detour_function), reinterpret_cast<LPVOID*>(og_function));

            if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED)
            {
                g_logger->send(levels::error, "Failed To Create Hook: {} Reason: {}", display, MH_StatusToString(status));
                VirtualProtect(address, sizeof(LPVOID), oldProtect, &oldProtect);
                return status;
            }

            g_logger->send(levels::debug, "Detouring: {}", display);

            status = MH_EnableHook(address);
            if (status != MH_OK)
            {
                g_logger->send(levels::error, "Failed Enable Detour: {} Reason: {}", display, MH_StatusToString(status));
                VirtualProtect(address, sizeof(LPVOID), oldProtect, &oldProtect);
                return status;
            }

            // Add the hook if it gets this far to the detoured list
            detoured_hooks.push_back({ display, address });

            // Revert memory protection after setting the hook
            VirtualProtect(address, sizeof(LPVOID), oldProtect, &oldProtect);

            // Send hooked message.
            g_logger->send(levels::debug, "Detoured: {} Successfully! ({})", display, address);

            return status;
        }


        static void unhook()
        {
            for (auto hook : get_detoured())
            {
                auto status = MH_DisableHook(hook.detour_address);
                if (status != MH_OK)
                    g_logger->send(levels::critical,"Failed To Disable: {} Reason: {}", hook.display_name, MH_StatusToString(status));

                g_logger->send(levels::debug,"Disabled Detour: {}", hook.display_name);
                
                status = MH_RemoveHook(hook.detour_address);
                if (status != MH_OK)
                    g_logger->send(levels::critical, "Failed To Remove: {} Reason: {}", hook.display_name, MH_StatusToString(status));
                
                g_logger->send(levels::debug,"Removed Detour: {}", hook.display_name);
            }

            auto status = MH_Uninitialize();
            if (status != MH_OK)
                g_logger->send(levels::critical, "Minhook Uninitialize Failed: {}", MH_StatusToString(status));

            g_logger->send(levels::success, "Minhook Uninitialized");
        }

        static std::vector<Template> get_detoured()
        {
            return detoured_hooks;
        }
    private:
        static inline std::vector<Template> detoured_hooks;
    };
    inline DetourManager g_Detour{};

    template<typename T>
    inline MH_STATUS detour(const std::string& display, LPVOID address, T** og_function, T* detour_function) noexcept
    {
        return g_Detour.detour<T>(display, address, og_function, detour_function);
    }

}