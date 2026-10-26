#pragma once
#include <string>
#include <vector>

namespace flow
{
    namespace util
    {
        void string_split(const std::string &in, std::vector<std::string> *pOut, const char &sep);
    }
}
