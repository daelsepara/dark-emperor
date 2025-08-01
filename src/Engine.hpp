#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <algorithm>
#include <string>

namespace Hex::Engine
{
    // change string case using input transform function (functptr)
    // see: https://www.geeksforgeeks.org/function-pointer-in-cpp/
    std::string ChangeCase(std::string str, int (*funcptr)(int))
    {
        auto transformed = str;

        std::transform(transformed.begin(), transformed.end(), transformed.begin(), funcptr);

        return transformed;
    }

    // compares two strings
    bool Compare(std::string s1, std::string s2, int (*funcptr)(int))
    {
        Engine::ChangeCase(s1, funcptr);

        Engine::ChangeCase(s2, funcptr);

        return (s1.compare(s2) == 0);
    }

    // changes all of the characters in the string to upper case
    std::string ToUpper(std::string str)
    {
        return Engine::ChangeCase(str, ::toupper);
    }

    // changes all of the characters in the string to lower case
    std::string ToLower(std::string str)
    {
        return Engine::ChangeCase(str, ::tolower);
    }
}

#endif
