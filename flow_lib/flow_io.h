#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "flow_color.h"

namespace flow
{
    namespace io
    {
        enum ConsoleStyle : uint8_t
        {
            CONSOLE_STYLE_NONE,
            CONSOLE_STYLE_BOLD,
            CONSOLE_STYLE_DIMMED,
            CONSOLE_STYLE_ITALIC,
            CONSOLE_STYLE_UNDERLINE,
            CONSOLE_STYLE_BLINK,
            CONSOLE_STYLE_RAPIDBLINK,
            CONSOLE_STYLE_INVERSE,
            CONSOLE_STYLE_HIDE,
            CONSOLE_STYLE_STRIKETHROUGH
        };

        struct ConsoleEffect
        {
            flow::Color foreColor;
            flow::Color backColor;
            std::vector<ConsoleStyle> styles;

            ConsoleEffect(
                const uint8_t fore_r = 255U,
                const uint8_t fore_g = 255U,
                const uint8_t fore_b = 255U,
                const uint8_t back_r = 0U,
                const uint8_t back_g = 0U,
                const uint8_t back_b = 0U,
                const std::vector<ConsoleStyle> &stylesIn = {CONSOLE_STYLE_NONE}) : foreColor(Color(fore_r, fore_b, fore_g)), backColor(Color(back_r, back_g, back_b)), styles(stylesIn) {}

            ConsoleEffect(
                const Color foregroundColor = Color(),
                const Color backgroundColor = Color(0, 0, 0),
                const std::vector<ConsoleStyle> stylesIn = {CONSOLE_STYLE_NONE}) : foreColor(foregroundColor), backColor(backgroundColor), styles(stylesIn) {}

            std::string ToString() const
            {
                std::string ret = "\033[";

                for (auto style : styles)
                {
                    ret += std::to_string(style) + ";";
                }

                ret += "38;2;" + std::to_string(foreColor.r) + ";" + std::to_string(foreColor.g) + ";" + std::to_string(foreColor.b) + ";";
                ret += "48;2;" + std::to_string(backColor.r) + ";" + std::to_string(backColor.g) + ";" + std::to_string(backColor.b) + ";";

                ret += "m";

                return ret;
            }

            std::string ToString(const std::string &applyTo) const
            {
                return ToString() + applyTo + GetReset();
            }

            static std::string GetReset()
            {
                return "\033[0m";
            }
        };

        class Console
        {
        public:
            static void WriteLine(const std::string &msg)
            {
                std::cout << msg << std::endl;
            }

            static void WriteLine(const std::string &msg, const ConsoleEffect &color)
            {
                std::cout << color.ToString(msg) << std::endl;
            }

            static void Write(const std::string &msg)
            {
                std::cout << msg;
            }

            static void Write(const std::string &msg, const ConsoleEffect &color)
            {
                std::cout << color.ToString(msg);
            }

            static void Clear()
            {
                std::cout << "\033[H\033[2J" << std::flush;
            }

            static std::string ReadLine()
            {
                std::string input;
                std::getline(std::cin, input);
                return input;
            }
        };

        class Debug
        {
        public:
            static void WriteLine(const std::string &msg)
            {
                std::cerr << "[DEBUG]: " << msg << std::endl;
            }

            static void WriteLine(const std::string &msg, const ConsoleEffect &color)
            {
                std::cerr << "[DEBUG]: " << color.ToString() + msg + color.GetReset() << std::endl;
            }

            static void Write(const std::string &msg)
            {
                std::cerr << "[DEBUG]: " << msg;
            }

            static void Write(const std::string &msg, const ConsoleEffect &color)
            {
                std::cerr << "[DEBUG]: " << color.ToString() + msg + color.GetReset();
            }
        };
    }
}