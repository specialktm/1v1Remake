#include "../renderer.hpp"
#include "../../Features/features.h"
#include "../../util/enums.h"

namespace cheat
{
	std::vector<std::pair<enums::HumanBodyBones, enums::HumanBodyBones>> BonePair
	{	
		// Right Leg
		{enums::HumanBodyBones::RightToes,enums::HumanBodyBones::RightFoot},
		{enums::HumanBodyBones::RightFoot,enums::HumanBodyBones::RightLowerLeg},
		{enums::HumanBodyBones::RightLowerLeg,enums::HumanBodyBones::RightUpperLeg},
		{enums::HumanBodyBones::RightUpperLeg,enums::HumanBodyBones::Hips},
		// Left Leg
		{enums::HumanBodyBones::LeftToes,enums::HumanBodyBones::LeftFoot},
		{enums::HumanBodyBones::LeftFoot,enums::HumanBodyBones::LeftLowerLeg},
		{enums::HumanBodyBones::LeftLowerLeg,enums::HumanBodyBones::LeftUpperLeg},
		{enums::HumanBodyBones::LeftUpperLeg,enums::HumanBodyBones::Hips},
		// Torso
		{enums::HumanBodyBones::Hips, enums::HumanBodyBones::Spine},
		{enums::HumanBodyBones::Spine, enums::HumanBodyBones::Chest},
		{enums::HumanBodyBones::Chest, enums::HumanBodyBones::UpperChest},
		{enums::HumanBodyBones::UpperChest, enums::HumanBodyBones::Neck},
		// Neck and Head and Eyes
		{enums::HumanBodyBones::Neck, enums::HumanBodyBones::Head},
		// Right Arm
		{enums::HumanBodyBones::UpperChest, enums::HumanBodyBones::RightShoulder},
		{enums::HumanBodyBones::RightShoulder, enums::HumanBodyBones::RightUpperArm},
		{enums::HumanBodyBones::RightUpperArm, enums::HumanBodyBones::RightLowerArm},
		{enums::HumanBodyBones::RightLowerArm, enums::HumanBodyBones::RightHand},
		// Left Arm
		{enums::HumanBodyBones::UpperChest, enums::HumanBodyBones::LeftShoulder},
		{enums::HumanBodyBones::LeftShoulder, enums::HumanBodyBones::LeftUpperArm},
		{enums::HumanBodyBones::LeftUpperArm, enums::HumanBodyBones::LeftLowerArm},
		{enums::HumanBodyBones::LeftLowerArm, enums::HumanBodyBones::LeftHand},

		// Left Fingers
		{enums::HumanBodyBones::LeftHand, enums::HumanBodyBones::LeftThumbProximal},
		{enums::HumanBodyBones::LeftThumbProximal, enums::HumanBodyBones::LeftThumbIntermediate},
		{enums::HumanBodyBones::LeftThumbIntermediate, enums::HumanBodyBones::LeftThumbDistal},

		{enums::HumanBodyBones::LeftHand, enums::HumanBodyBones::LeftIndexProximal},
		{enums::HumanBodyBones::LeftIndexProximal, enums::HumanBodyBones::LeftIndexIntermediate},
		{enums::HumanBodyBones::LeftIndexIntermediate, enums::HumanBodyBones::LeftIndexDistal},

		{enums::HumanBodyBones::LeftHand, enums::HumanBodyBones::LeftMiddleProximal},
		{enums::HumanBodyBones::LeftMiddleProximal, enums::HumanBodyBones::LeftMiddleIntermediate},
		{enums::HumanBodyBones::LeftMiddleIntermediate, enums::HumanBodyBones::LeftMiddleDistal},

		{enums::HumanBodyBones::LeftHand, enums::HumanBodyBones::LeftRingProximal},
		{enums::HumanBodyBones::LeftRingProximal, enums::HumanBodyBones::LeftRingIntermediate},
		{enums::HumanBodyBones::LeftRingIntermediate, enums::HumanBodyBones::LeftRingDistal},

		{enums::HumanBodyBones::LeftHand, enums::HumanBodyBones::LeftLittleProximal},
		{enums::HumanBodyBones::LeftLittleProximal, enums::HumanBodyBones::LeftLittleIntermediate},
		{enums::HumanBodyBones::LeftLittleIntermediate, enums::HumanBodyBones::LeftLittleDistal},

		// Right Fingers
		{enums::HumanBodyBones::RightHand, enums::HumanBodyBones::RightThumbProximal },
		{enums::HumanBodyBones::RightThumbProximal, enums::HumanBodyBones::RightThumbIntermediate},
		{enums::HumanBodyBones::RightThumbIntermediate, enums::HumanBodyBones::RightThumbDistal},
		
		{enums::HumanBodyBones::RightHand, enums::HumanBodyBones::RightIndexProximal},
		{enums::HumanBodyBones::RightIndexProximal, enums::HumanBodyBones::RightIndexIntermediate},
		{enums::HumanBodyBones::RightIndexIntermediate, enums::HumanBodyBones::RightIndexDistal},
		
		{enums::HumanBodyBones::RightHand, enums::HumanBodyBones::RightMiddleProximal},
		{enums::HumanBodyBones::RightMiddleProximal, enums::HumanBodyBones::RightMiddleIntermediate},
		{enums::HumanBodyBones::RightMiddleIntermediate, enums::HumanBodyBones::RightMiddleDistal},
		
		{enums::HumanBodyBones::RightHand, enums::HumanBodyBones::RightRingProximal},
		{enums::HumanBodyBones::RightRingProximal, enums::HumanBodyBones::RightRingIntermediate},
		{enums::HumanBodyBones::RightRingIntermediate, enums::HumanBodyBones::RightRingDistal},
		
		{enums::HumanBodyBones::RightHand, enums::HumanBodyBones::RightLittleProximal},
		{enums::HumanBodyBones::RightLittleProximal, enums::HumanBodyBones::RightLittleIntermediate},
		{enums::HumanBodyBones::RightLittleIntermediate, enums::HumanBodyBones::RightLittleDistal}

	};

	Unity::CGameObject* Player;
	void renderer::ESP()
	{
			if (PlayerList.size() > 0)
			{
				void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
				for (int i = 0; i < PlayerList.size(); i++)
				{
					if (!PlayerList[i])
						continue;

					Player = PlayerList[i];
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
					bool playerIsBot = playerCurrentContoller->CallMethodSafe<bool>("get_IsBot");
					
					t_PlayerInfo* PlayerInfo = playerCurrentContoller->CallMethodSafe<t_PlayerInfo*>("get_PlayerInfo");
					if (PlayerInfo == nullptr || PlayerInfo->fields.Username == nullptr)
						continue;
					std::string_view PlayerUsername = util::SystemString(PlayerInfo->fields.Username);

		
					if (!playerIsBot)
					{
						if (i >= static_cast<int>(playerMenu.size()))
						{
							playerMenu.resize(i + 1);
						}

						if (playerMenu[i].name.empty())
						{
							t_playersubmenu temp;
							temp.name = !PlayerUsername.empty() ? PlayerUsername : "Empty Name: Error";
							temp.uuid = util::SystemString(PlayerInfo->fields.UUID);
							temp.skin = util::SystemString(PlayerInfo->fields.PlayerSkin);
							playerMenu[i] = temp;
						}
					}
\

					auto playerPosition = PlayerList[i]->GetTransform()->GetPosition();

					if (util::SystemString(PlayerInfo->fields.Username) == "CrimsonVeins" || !PlayerUsername.empty())
						continue;

					for (std::pair<enums::HumanBodyBones, enums::HumanBodyBones> bone_index : BonePair)
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
								imgui::GetForegroundDrawList()->AddLine(ImVec2{bone1Screen.x ,bone1Screen.y}, ImVec2{bone2Screen.x ,bone2Screen.y}, IM_COL32_WHITE, 1.5f);
							}
						}
					}
					if (features::BoxEsp)
					{
						Unity::Vector3 root_pos = playerPosition;

						Unity::Vector3 HeadPosition{ root_pos.x, root_pos.y + 1.703f, root_pos.z };

						Unity::Vector3 HeadAimPosition{ root_pos.x, root_pos.y + 1.1f, root_pos.z };

						Vector2 PlayerHeadPosition;
						Vector2 PlayerFeetPosition;

						if (util::WorldToScreen(HeadPosition, PlayerHeadPosition) && util::WorldToScreen(root_pos, PlayerFeetPosition))
						{
							const float height = PlayerFeetPosition.y - PlayerHeadPosition.y;
							const float width = height * 0.2f;
							const auto left = static_cast<float>(PlayerHeadPosition.x - width);
							const auto right = static_cast<float>(PlayerHeadPosition.x + width);

							ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ left, PlayerHeadPosition.y }, ImVec2{ right, PlayerFeetPosition.y }, IM_COL32_WHITE);
						}
					}
	
				}	
				IL2CPP::Thread::Detach(m_pThisThread);
			}
		}
	
}
