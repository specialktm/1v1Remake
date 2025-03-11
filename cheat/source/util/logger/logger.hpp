#pragma once
/*
* File Name: Logger.hpp
* Purpose: Contains the function definitions for the logger.
* Author VoDKa
* Date: 12.07.2024 | EU Format
* Time: 01:48
*/

// Includes
#define APP_NAME "Cortez"
#ifdef Developer
#define APP_VERSION "Developer"
#else
#define APP_VERSION "1.0.0"
#endif
#include "../../pch/pch.h"


namespace cheat
{

    enum class levels
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
            this->initialize(APP_NAME);
        }
        ~logger() 
        {
            this->flush();
            this->uninitialize(); 
        }
    public:

        void set_level(levels level);
        std::string level_to_string(levels level);

        template <typename... args>
        void send(levels level, const std::string& format, args... msg)
        {
            std::string formatted = std::vformat(format, std::make_format_args(msg...));
            old_send(level, formatted);
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
        void old_send(levels level, const std::string& msg);

        levels m_log_level = levels::info;
        FILE* m_file;
        HANDLE h_console_out = nullptr;
        DWORD m_console_mode = 0;
    };

    inline std::unique_ptr<logger> g_logger;
}
