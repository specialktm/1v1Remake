#include "../pch/pch.h"
#include "../cheat/main.h"

int __stdcall DllMain(HMODULE hModule, unsigned long ul_reason_for_call, void* lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        g_Module = hModule;
        DisableThreadLibraryCalls(g_Module);
        g_MainThread = CreateThread(nullptr, NULL, &cheat::main_entry, nullptr, NULL, nullptr);
    }
    return 1;
}