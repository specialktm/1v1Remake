#pragma once
/*
* File Name: Logger.hpp
* Purpose: Contains the function definitions for the logger.
* Author VoDKa
* Date: 12.07.2024 | EU Format
* Time: 01:48
*/

#define WIN32_LEAN_AND_MEAN
#define STRING_IMPL(x) #x
#define WSTRING_IMPL(x) L ## x
#define WIDE_IMPL(x) WSTRING_IMPL(x)
#define STR_IMPL(x) STRING_IMPL(x)
// Includes
#define APP_NAME "Cotez"
#ifdef _DEBUG
#define APP_VERSION "Developer"
#else
#define APP_VERSION "1.0.0"
#endif
#include <Windows.h>

#include <string_view>
#include <format>
#include <source_location>
#include <sysinfoapi.h>

enum class levels
{
    developer = 0,
    debug,
    info,
    success,
    warning,
    error,
    critical,
    custom
};
enum class eColors
{
    developer = 0,
    debug,
    info,
    success,
    warning,
    error,
    critical
};


class logger {
public:
    logger() : m_file(nullptr)
    {
        this->initialize(APP_NAME " " APP_VERSION);
    }
    ~logger()
    {
        this->flush();
        this->uninitialize();
    }
public:

    void set_level(levels level);
    std::string_view level_to_string(levels level);
    std::string_view ColorToString(eColors Color);

    void UpdateLine(const std::string_view& oldText, const std::string_view& newText);

	template <typename... args>
    void send(levels level, const std::string_view& format, args... msg)
    {
        std::string formatted = std::vformat(format, std::make_format_args(msg...));
        if (level >= m_log_level)
        {
            std::string_view ColoredLevel{ level_to_string(level) };
            std::string log_message = std::string(ColoredLevel) + " " + formatted;
            ToConsole(log_message);
        }
    }

	template <typename... args>
    void send(const  std::source_location location,levels level, const std::string_view& format, args... msg)
    {
        std::string formatted = std::vformat(format, std::make_format_args(msg...));
        if (level >= m_log_level)
        {
            std::string_view ColoredLevel{ level_to_string(level) };
            std::string log_message = std::string(ColoredLevel) + " " + formatted;
            ToConsole(log_message, location);
        }
    }

    template <typename... args>
    void Custom(const std::source_location location, eColors color, const std::string_view& tag, const std::string_view& format, args... msg)
    {
        if (color >= m_color_level)
        {
            std::string formatted = std::vformat(format, std::make_format_args(msg...));
            std::string ClearColor(color == eColors::debug ? "]\33[m " : "] ");
            std::string custom_message = { std::string(ColorToString(color)) + "[" + std::string(tag) + ClearColor + formatted };
            ToConsole(custom_message,location);
        }
    }

    template <typename... args>
    void Custom(eColors color, const std::string_view& tag, const std::string_view& format, args... msg)
    {
        if (color >= m_color_level)
        { // 7 invert foreground
            std::string formatted = std::vformat(format, std::make_format_args(msg...));
            std::string ClearColor(color == eColors::debug ? "]\33[m " : "] ");
            std::string custom_message = { std::string(ColorToString(color)) + "[" + std::string(tag) + ClearColor + formatted };
            ToConsole(custom_message);
        }
    }

    void GetTimestamp(char* buffer, size_t size) const
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        snprintf(buffer, size, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    }

private:
    void flush();
    void initialize(const char* title);
    void uninitialize();
    void ToConsole(const std::string_view& msg, const std::source_location location);
    void ToConsole(const std::string_view& msg);

    levels m_log_level = levels::info;
    eColors m_color_level = eColors::developer;
    FILE* m_file;
    HANDLE h_console_out = nullptr;
    DWORD m_console_mode = 0;
};

inline std::unique_ptr<logger> g_logger;

