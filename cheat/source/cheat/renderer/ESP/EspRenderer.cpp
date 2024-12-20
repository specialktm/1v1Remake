#include "../renderer.hpp"
#include "../../Features/features.h"
namespace cheat
{
	std::vector<std::pair<int, int>> bone_pair{
		// Right Leg
		{21,6},
		{6,4},
		{4,2},
		{2,0},
		// Left Leg
		{20, 5},
		{5, 3},
		{3, 1},
		{1, 0},
		// Torso
		{0, 7},
		{7, 8},
		{8, 9},
		{9, 10},
		// Neck and Head
		{10, 11}, // 11 is head bone
		// Right Arm
		{9, 13},
		{13, 15},
		{15, 17},
		{17, 19},
		// Left Arm
		{9,12},
		{12,14},
		{14,16},
		{16,18},

		// Left Fingers 
		{18,25},
		{25,26},
		{26,27},
		///////
		{18,35},
		{35,36},
		{36,37},
		{18,28},
		{28,29},
		{29,30},
		///////
		{18,37},
		{37,38},
		{38,39},
		//{18,29},
		// Right Fingers 
		{19,40},
		{40,41},
		{41,42},
		{19,43},
		{43,44},

	};

	void renderer::ESP()
	{
			if (PlayerList.size() > 0)
			{
				void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
				for (int i = 0; i < PlayerList.size(); i++)
				{
					if (!PlayerList[i])
						continue;

					Unity::CComponent* playerCurrentContoller = PlayerList[i]->GetComponent("PlayerController");
					Unity::CComponent* playerCurrentAnimator = PlayerList[i]->GetComponent("Animator");

					if (!playerCurrentContoller)
						continue;
					if (!playerCurrentAnimator)
						continue;

					bool playerIsDowned = playerCurrentContoller->CallMethodSafe<bool>("get_IsDowned");


					bool playerIsDead = playerCurrentContoller->CallMethodSafe<bool>("get_IsDead");
					if (playerIsDead)
						continue;

					bool playerIsTeam = playerCurrentContoller->CallMethodSafe<bool>("get_IsTeammate");
					if (playerIsTeam)
						continue;

					auto playerPosition = PlayerList[i]->GetTransform()->GetPosition();

					for (std::pair<int, int> bone_index : bone_pair)
					{
						auto bone1 = playerCurrentAnimator->CallMethodSafe<Unity::CTransform*>("GetBoneTransformInternal", bone_index.first);
						auto bone2 = playerCurrentAnimator->CallMethodSafe<Unity::CTransform*>("GetBoneTransformInternal", bone_index.second);

						if (!bone1 || !bone2)
							continue;

						auto bone1Position = bone1->GetPosition();
						auto bone2Position = bone2->GetPosition();

						Vector2 bone1Screen;
						Vector2 bone2Screen;

						if (util::WorldToScreen(bone1Position, bone1Screen) && util::WorldToScreen(bone2Position, bone2Screen))
						{
							if (features::SkeletonEsp)
							{
								Menu.m_DrawList->AddLine(ImVec2{ bone1Screen.x ,bone1Screen.y }, ImVec2{ bone2Screen.x ,bone2Screen.y }, IM_COL32_WHITE, 1.5f);
							}
						}
					}
				}
				IL2CPP::Thread::Detach(m_pThisThread);
			}
		}
	
}
