#include "hooking.h"
#include "methods/detour.h"
#include "../../kiero/kiero.h"
#include "../../fiber/Manager.h"


namespace cheat
{
	void InitPlayerList()
	{
		while (true)
		{

			void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
			PlayerList.clear();
			auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>("PlayerController");
			for (int i = 0; i < list->m_uMaxLength; i++)
			{
				if (!list->operator[](i))
				{
					continue;
				}

				PlayerList.push_back(list->operator[](i)->GetGameObject());

			}
			IL2CPP::Thread::Detach(m_pThisThread);
			Fiber::get()->yield(1s);
		}
	}

	hooking::hooking()
	{
		auto status = MH_Initialize();
		if (status != MH_OK)
		{
			g_logger->send(levels::critical, "MinHook Initialize Failed: {}", MH_StatusToString(status));
		}

		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			g_logger->send(levels::debug, "DX11");
		}

		if (IL2CPP::Initialize(true))
		{
			g_logger->send(levels::success, "IL2CPP Initialized");
		}
		IL2CPP::Callback::Initialize();

		offsets::GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
		offsets::UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");

		Unity::il2cppClass* ThirdPersonCameraClass = IL2CPP::Class::Find("vThirdPersonCamera");
		offsets::AddRecoil = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(ThirdPersonCameraClass, "AddRecoil");

		g_logger->send(levels::developer, "Offsets:: {:#X} | {:#X} | {:#X}", offsets::GameAssembly, offsets::UnityPlayer, offsets::AddRecoil - offsets::GameAssembly);


		fiber_manager::add_fiber("PlayerListCache", &InitPlayerList);
		this->Hook();
	}
	hooking::~hooking()
	{

		this->UnHook();
	}

	void hooking::Hook()
	{
		// D3D11
		kiero::bind(8, (void**)&hooks::ogPresent, hooks::Present);
		// Detour
		detour("AddRecoil", (LPVOID)offsets::AddRecoil, &hooks::ogAddRecoil, hooks::AddRecoil);
	}

	void hooking::UnHook()
	{
		IL2CPP::Callback::Uninitialize();
		g_Detour.unhook();

	}

}