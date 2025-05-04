#pragma once
#include <vector>
#include "Types/BaseOption.hpp"
#include <stdexcept>
#include <d3d11.h>
#include "../../../imgui/imgui.h"
namespace cheat
{
	inline std::vector<BaseOption> OptionsVector;
	inline std::vector<int> OptionsSelectableVector;
	inline float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}
	class Interface
	{
		class ListInterface
		{
		private:
			struct tHeader
			{
				float Height = 100.f;
				const char* Text = "Header";
				ID3D11ShaderResourceView* Image = nullptr;
				int Frame;
				float Rounding = 4.f;
				bool IsRounded = false;
				bool ShouldDrawText = true;
				ImColor Color{0, 0, 0, 255};
			};
			struct tSubtitle
			{
				float Height = 100.f;
				const char* Text = "Main";
				ImColor Color{0, 0, 0, 255};
				void SetText(const char* pNewText)
				{
					Text = pNewText;
				}

			};
			struct tBackground
			{
				ImColor Color{ 0,0,0,180};
			};
			struct tScroller
			{
				float Smoothing = 0.01f;

				ImColor Color{140,0,0,255};
			};
			struct tFooter
			{
				const char* Text = "Footer";
				const char* Version = "1.1.1";
				float FooterHeight = 10.f;
				ImColor Color{ 0,0,0,255 };
			};
		public:
			int NumberOfOptions = 0;
			int NumberOfOptionsSelectable = 0;
			int MaxNumberOfOptionsVisible = 12;
			float MenuWidth = 420.f;
			float OptionHeight = 40.f;
			ImVec2 Position{0.05f, 0.05f};
			ImVec2 DrawPosition{80.f, 80.f};
		public:
			tHeader Header;
			tSubtitle Subtitle;
			tBackground Background;
			tScroller Scroller;
			tFooter Footer;
		public:
			void Render();
			void RenderHeader();
			void RenderSubtitle();
			void RenderBackground();
			void RenderFooter();
			void HandleTheInput();
			size_t GetOptionCount()
			{
				return OptionsVector.size();
			}
			size_t SelectableCount()
			{
				return OptionsSelectableVector.size();
			}
			BaseOption& GetOption(size_t pIndex)
			{
				return OptionsVector.at(pIndex);
			}
			int GetSelectable(size_t pIndex)
			{
				return OptionsSelectableVector.at(pIndex);
			}
		private:
			ImGuiIO* IO = &ImGui::GetIO();
			ImDrawList* BackgroundDrawlist = ImGui::GetBackgroundDrawList();
			ImDrawList* ForegroundDrawlist = ImGui::GetForegroundDrawList();
			int m_Selected = 1;
			const char* m_SelectedId = "";
			int m_Index = 1;
		};

		class ClickInterface
		{
		public:
			float BackgroundColor[3]{ 1.f, 1.f, 1.f };
		public:
			void Render();
		};
	public:
		enum class InterfaceType : int
		{
			ListInterface = 0,
			ClickInterface
		};
	public:
		Interface() = default;
		~Interface() = default;
	public:
		void RenderInterface();
		void SwitchInterface(InterfaceType pInterfaceType)
		{
			m_InterfaceType = pInterfaceType;
		}
		void AddOption(BaseOption pOption)
		{
			OptionsVector.emplace_back(pOption);
			ListInterface.NumberOfOptions += 1;
			if (pOption.IsSelecable())
				OptionsSelectableVector.emplace_back(OptionsVector.size() - 1);
		}
	public:
		bool GetShouldRenderInterface()
		{
			return m_ShouldRenderInterface;
		}
		
		void SetShouldRenderInterface(bool pShouldRenderInterface)
		{
			m_ShouldRenderInterface = pShouldRenderInterface;
		}
	public:
		ListInterface ListInterface;
		ClickInterface ClickInterface;
	private:
		InterfaceType m_InterfaceType = InterfaceType::ListInterface;
		bool m_ShouldRenderInterface = true;
	};


}