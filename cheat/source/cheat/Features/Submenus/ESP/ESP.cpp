#include "../../../renderer/Menu/Submenu/Submenu.h"
#include "../../features.h"

namespace cheat
{
	float GetDeltaTime()
	{
		high_resolution_clock::time_point lastTime;
		auto currentTime = high_resolution_clock::now(); // Get the current time
		duration<float> delta = currentTime - lastTime; // Calculate the difference
		lastTime = currentTime; // Update the last time to the current time for the next frame
		return delta.count(); // Return delta time in seconds (as a float)
	}

	bool features::ExecAimbot(Unity::CGameObject* target, Vector2 playerHead)
	{
		static_assert(std::is_same_v<decltype(LastPlayer), Unity::CGameObject*>, "Type mismatch!");
		static_assert(std::is_same_v<decltype(target), Unity::CGameObject*>, "Type mismatch!");

		if (!target && !LastPlayer)
		{
			return false;
		}
		if (features::Aimbot)
		{
			if (playerHead.x > (g_Renderer->ScreenCenter.x + 80.f))
				return false;
			if (playerHead.x < (g_Renderer->ScreenCenter.x - 80.f))
				return false;
			if (playerHead.y > (g_Renderer->ScreenCenter.y + 80.f))
				return false;
			if (playerHead.y < (g_Renderer->ScreenCenter.y - 80.f))
				return false;
		}

		if (GetAsyncKeyState(VK_RBUTTON))
		{

			if (target == LastPlayer)
			{
				auto playerCurrentAnimator = target->GetComponent("Animator");
				if (!playerCurrentAnimator)
					return false;

				Unity::Vector3 root_pos = target->GetTransform()->GetPosition();
				Unity::Vector3 targetPosition = playerCurrentAnimator->CallMethodSafe<Unity::CTransform*>("GetBoneTransformInternal", 11)->GetPosition();

				static Unity::Vector3 lastTargetPosition = targetPosition;
				Unity::Vector3 targetVelocity = (targetPosition - lastTargetPosition) / GetDeltaTime();
				lastTargetPosition = targetPosition;

				float predictionTime = 0.01f;
				Unity::Vector3 predictedPosition = targetPosition + (targetVelocity);

				Vector2 predictedScreenPos;
				if (!util::WorldToScreen(predictedPosition, predictedScreenPos))
					return false;

				static Vector2 lastMousePos;
				Vector2 smoothedPos;
				smoothedPos.y = lastMousePos.y + (predictedScreenPos.y - lastMousePos.y) / 1.f;
				smoothedPos.x = lastMousePos.x + (predictedScreenPos.x - lastMousePos.x) / 1.f;

				g_Renderer->Menu.m_DrawList->AddLine(ImVec2(predictedScreenPos.x, predictedScreenPos.y), ImVec2(g_Renderer->ScreenCenter.x, g_Renderer->ScreenCenter.y), IM_COL32(255,0,0,255));
				util::MouseMove(smoothedPos.x, smoothedPos.y, g_Renderer->ScreenSize.x, g_Renderer->ScreenSize.y, 1.f);
				lastMousePos = smoothedPos;
			}
		}
		else
		{
			LastPlayer = target;
		}
	}

	void Submenus::Menus::MenuESP()
	{
		Title("ESP");
		
		Bool("Aimbot", "For no skill", &features::Aimbot);
		Bool("Skeleton Esp", "Draw lines on the player skeleton", &features::SkeletonEsp);
		Bool("Box Esp", "Draw box around the player", &features::BoxEsp);
	}
}