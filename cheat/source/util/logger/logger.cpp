/*
* File Name: Logger.cpp
* Purpose: Contains the logic for the logger.
* Author VoDKa
* Date: 12.07.2024 | EU Format
* Time: 01:48
*/

#include <ios>

#include "logger.hpp"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>
// ***
namespace cheat {
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
        errno_t err = fopen_s(&m_file, "log.txt", "a+");

        if (err == 0 || m_file)
        {
            fseek(m_file, 0, SEEK_END);
            long size = ftell(m_file);
            fseek(m_file, 0, SEEK_SET);

            if (size == 0) {
                send(levels::error, "m_file is empty, no logs to write.");
                return;
            }

            fseek(m_file, 0, SEEK_SET);

            std::ofstream logFile{ "log.txt", std::ios::out | std::ios::in };
            if (logFile)
            {
                char buffer[1024];
                while (fgets(buffer, sizeof(buffer), m_file))
                {
                    logFile << buffer;
                }
                logFile.close();
            }

            fclose(m_file);
        }

        FreeConsole();
    }


    // g_logger.send(levels::trace,"Format Test {}","format");
    void logger::ToConsole(const std::string_view& msg, const std::source_location location)
    {
        errno_t err = fopen_s(&m_file, "log.txt", "a+");
        char buffer[64];
        GetTimestamp(buffer, sizeof(buffer));

        static const std::regex ansi_regex("\033\\[[0-9;]*m");
        auto LogMessage = std::regex_replace(msg.data(), ansi_regex, "");
        std::string file_name = std::filesystem::path(location.file_name()).filename().string();

        printf("[%s] \33[1m\33[38;5;124m[%s:%d]\33[m %s\33[m\n", buffer, file_name.c_str(), location.line(), msg.data());
        if (err == 0 || m_file)
        {
            fprintf(m_file, "[%s] %s\n", buffer, LogMessage.c_str());
            fflush(m_file);
            fclose(m_file);
        }
    }

    void logger::ToConsole(const std::string_view& msg)
    {
        errno_t err = fopen_s(&m_file, "log.txt", "a+");
        char buffer[64];
        GetTimestamp(buffer, sizeof(buffer));

        static const std::regex ansi_regex("\033\\[[0-9;]*m");
        auto LogMessage = std::regex_replace(msg.data(), ansi_regex, "");

        printf("[%s] %s\33[m\n", buffer, msg.data());
        if (err == 0 || m_file)
        {
            fprintf(m_file, "[%s] %s\n", buffer, LogMessage.c_str());
            fflush(m_file);
            fclose(m_file);
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
    std::string_view logger::level_to_string(levels level)
    {
        switch (level)
        {
            case levels::developer:
                return "\33[38;5;105m[Developer]";
            case levels::debug:
                return "\33[38;5;63;5m[Debug]\33[m";
            case levels::info:
                return "\33[1m\33[38;5;247m[Info]";
            case levels::success:
                return "\33[1m\33[38;5;119m[Success]";
            case levels::warning:
                return "\33[1m\33[38;5;136m[Warn]";
            case levels::error:
                return "\33[1m\33[38;5;196m[Error]\33[m";
            case levels::critical:
                return "\33[1m\33[38;5;124m[Critical]\33[m";
            case levels::custom:
                return "";
            default:
                return "Unhandled Level";
        }
    }
    void logger::UpdateLine(const std::string_view& oldText, const std::string_view& newText)
    {

        printf("%s -> %s\33[m\r", oldText.data(), newText.data());
        std::cout.flush();
    }
    std::string_view logger::ColorToString(eColors Color)
    {
        switch (Color)
        {
            case eColors::developer:
                return "\33[38;5;105m";
            case eColors::debug:
                return "\33[38;5;63;5m";
            case eColors::info:
                return "\33[1m\33[38;5;247m";
            case eColors::success:
                return "\33[1m\33[38;5;119m";
            case eColors::warning:
                return "\33[1m\33[38;5;136m";
            case eColors::error:
                return "\33[1m\33[38;5;196m";
            case eColors::critical:
                return "\33[1m\33[38;5;124m";
            default:
                return "Unhandled Color";
        }
    }
};