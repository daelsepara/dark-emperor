#ifndef __UTILITIES_HPP__
#define __UTILITIES_HPP__

#include <algorithm>
#include <string>

namespace DarkEmperor::Utilities
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
        Utilities::ChangeCase(s1, funcptr);

        Utilities::ChangeCase(s2, funcptr);

        return (s1.compare(s2) == 0);
    }

    // changes all of the characters in the string to upper case
    std::string ToUpper(std::string str)
    {
        return Utilities::ChangeCase(str, ::toupper);
    }

    // changes all of the characters in the string to lower case
    std::string ToLower(std::string str)
    {
        return Utilities::ChangeCase(str, ::tolower);
    }

    template <typename T>
    T ClipValue(T value, T min_value, T max_value)
    {
        return std::min(std::max(value, min_value), max_value);
    }

    std::string CleanString(std::string text, const char *chars)
    {
        for (auto i = 0; i < strlen(chars); ++i)
        {
            text.erase(std::remove(text.begin(), text.end(), chars[i]), text.end());
        }

        return text;
    }
}

#endif
