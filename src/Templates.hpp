#ifndef __TEMPLATES_HPP__
#define __TEMPLATES_HPP__

#include <string>
#include <unordered_map>
#include <vector>

namespace DarkEmperor
{
    // smart pointer
    template <typename T>
    using Smart = std::shared_ptr<T>;

    // array (vector of vectors)
    template <typename T>
    using Array = std::vector<std::vector<T>>;

    // vector of integers
    typedef std::vector<int> Integers;

    // search for a key in a vector of type/objects
    template <typename T, typename R = typename std::vector<T>::const_iterator>
    R Find(std::vector<T> &vector, T &key)
    {
        return std::find(vector.begin(), vector.end(), key);
    }

    template <typename T>
    bool Has(std::vector<T> &vector, T &key)
    {
        return DarkEmperor::Find(vector, key) != vector.end();
    }

    template <typename T>
    bool In(std::vector<T> &vector, T &key)
    {
        return DarkEmperor::Find(vector, key) != vector.end();
    }
}

#endif