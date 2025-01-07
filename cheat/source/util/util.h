#pragma once
#include "../fiber/Fiber.h"
#include "../pch/pch.h"
#include "Vector.hpp"
#include <Includes.hpp>



namespace cheat::timer {
	using namespace std::chrono;
	using namespace std::chrono_literals;
	class simpleTimer // Mister9982/99Anvar99 (Github) Aera
	{
	public:
		explicit simpleTimer(milliseconds delay) :
			m_Timer(high_resolution_clock::now()),
			m_Delay(std::chrono::duration_cast<high_resolution_clock::duration>(delay))
		{
		}

		bool update()
		{
			if (const auto now = high_resolution_clock::now(); (now.time_since_epoch() - m_Timer.time_since_epoch()).
				count() >= m_Delay.count())
			{
				m_Timer = now;
				return true;
			}

			return false;
		}

		void set_delay(int delay)
		{
			m_Delay = milliseconds(delay);
		}

		void set_delay(milliseconds delay)
		{
			m_Delay = delay;
		}

	private:
		high_resolution_clock::time_point m_Timer;
		high_resolution_clock::duration m_Delay;
	};
}

namespace cheat::util
{
	
	uint8_t char_to_byte(char c);
	std::optional<std::uint8_t> char_to_hex(const char c);

	template <typename T>
	concept Callable = std::is_invocable_v<T>;

	template <Callable Action>
	void do_timed(DWORD delay, Action&& action) {
		auto const start = std::chrono::high_resolution_clock::now();
		action();
		auto const end = std::chrono::high_resolution_clock::now();

		Fiber::get()->yield(std::chrono::milliseconds(delay) - (end - start));
	}

	uint32_t joaat(const std::string_view& text);

	template<typename... Args>
	inline std::string format_message(const std::string& message, const Args&... args)
	{
		return std::vformat(message, std::make_format_args(args...));
	}

	bool WorldToScreen(Unity::Vector3 world, Vector2& screen);

#define do_once(block)                     \
    do {                                   \
        static std::once_flag init_flag;   \
        std::call_once(init_flag, block);  \
    } while (0)

	template <typename... Args>
	std::string combine_strings(Args&&... args) {
		static auto combine = [](const auto&... strs) -> std::string {
			return (std::string(strs) + ...);
		};
		return combine(std::forward<Args>(args)...);
	}
}
