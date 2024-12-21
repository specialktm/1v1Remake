/*
* File Name: Logger.cpp
* Purpose: Contains the logic for the logger.
* Author VoDKa
* Date: 12.07.2024 | EU Format
* Time: 01:48
*/

#include "logger.hpp"
// ***
namespace cheat
{
    // initialize logger
    void logger::initialize(const char* title)
    {

        if (!AllocConsole())
            MessageBoxA(nullptr, "The console window was not created", "Uh Oh", MB_ICONEXCLAMATION);

        freopen_s(&m_file, "CONOUT$", "w", stdout);
        freopen_s(&m_file, "CONOUT$", "w", stderr);
        freopen_s(&m_file, "CONIN$", "r", stdin);


        h_console_out = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h_console_out == INVALID_HANDLE_VALUE)
        {
            MessageBox(NULL, L"Invalid handle value for output", NULL, MB_ICONEXCLAMATION);
            return;
        }

        if (!GetConsoleMode(h_console_out, &m_console_mode))
        {
            MessageBox(NULL, L"Failed to get console mode for output", NULL, MB_ICONEXCLAMATION);
            return;
        }

        m_console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(h_console_out, m_console_mode))
        {
            MessageBox(NULL, L"Failed to set console mode for output", NULL, MB_ICONEXCLAMATION);
            return;
        }

        SetStdHandle(STD_OUTPUT_HANDLE, h_console_out);
        SetStdHandle(STD_ERROR_HANDLE, h_console_out);
        SetStdHandle(STD_INPUT_HANDLE, GetStdHandle(STD_INPUT_HANDLE));

        SetConsoleTitleA(title);
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 230, LWA_ALPHA);
        
#ifdef Developer
        set_level(levels::developer);
#endif
        send(levels::info, "Logger Created: {}", title);

    }

    // uninitialize logger
    void logger::uninitialize()
    {   
        send(levels::info, "Goodbye, You may now close this window.");

        if (m_file)
        {
            fclose(m_file);
        }
        FreeConsole();
    }

    // Old Logger Function. // Still used for the format.
    // g_logger.send(levels::trace,"Format Test {}","format");
    void logger::old_send(levels level, const std::string& msg)
    {
        if (level >= m_log_level)
        {
            char buffer[64];
            GetTimestamp(buffer, sizeof(buffer));
            std::string newMsg{ level_to_string(level) + " " + msg + "\33[m"};
            printf("[%s] %s\n", buffer, newMsg.c_str());
            if (m_file)
            {
                fprintf(m_file, "[%s] %s\n", buffer, msg.c_str());
                fflush(m_file);
            }  
        }       
    }
    // Flush the queue.
    void logger::flush()
    {
        fflush(m_file);
    }
    // Setting The Log Level.
    void logger::set_level(levels level)
    {
        m_log_level = level;
    }

    // Convert enum class levels to strings and color them.
    std::string logger::level_to_string(levels level)
    {
        switch (level)
        {
            case levels::developer:
                return "\33[38;5;105m[Developer]";
            case levels::debug:
                return "\33[38;5;63;5m[Debug]\33[m";
            case levels::info:
                return "\33[1m\33[38;5;237m[Info]";
            case levels::success:
                return "\33[1m\33[38;5;119m[Success]";
            case levels::warn:
                return "\33[1m\33[38;5;136m[Warn]";
            case levels::error:
                return "\33[1m\33[38;5;196m[Error]\33[m";
            case levels::critical:
                return "\33[1m\33[38;5;124m[Critical]\33[m";
            default:
                return "Unhandled Level";
        }
    }
}