#pragma once
#include <vector>
#include "Types/option.hpp"

namespace cheat
{

	class Interface
	{
		struct ListInterface
		{
		private:
			struct tHeader
			{
				float HeaderHeight = 60.f;
			};
			struct tBackground
			{
			};
			struct tScroller
			{
			};
			struct tFooter
			{
				float FooterHeight = 10.f;
			};
		public:
			int NumberOfOptions = 0;
			int NumberOfOptionsVisible = 0;
			int MaxNumberOfOptionsVisible = 12;

			tHeader Header;
			tBackground Background;
			tScroller Scroller;
			tFooter Footer;

			std::vector<BaseOption> OptionsVector;
		};

		struct ClickInterface
		{
		public:
			float BackgroundColor[3]{ 1.f, 1.f, 1.f };
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
	public:
		bool GetShouldRenderInterface()
		{
			return m_ShouldRenderInterface;
		}
		void SetShouldRenderInterface(bool pShouldRenderInterface)
		{
			m_ShouldRenderInterface = pShouldRenderInterface;
		}
	public: // Helper Functions
		inline float lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}
	private:
		void RenderListInterface();
		void RenderClickInterface();
	public:
		ListInterface ListInterfaceSettings;
		ClickInterface ClickInterfaceSettings;
	private:
		InterfaceType m_InterfaceType = InterfaceType::ListInterface;
		bool m_ShouldRenderInterface = false;
	};


}