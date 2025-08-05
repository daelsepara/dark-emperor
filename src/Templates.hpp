#ifndef __TEMPLATES_HPP__
#define __TEMPLATES_HPP__

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace DarkEmperor
{
    // smart pointer
    template <typename T>
    using Smart = std::shared_ptr<T>;

    // vector of things
    template <typename T>
    using List = std::vector<T>;

    // array (vector of vectors)
    template <typename T>
    using Array = List<List<T>>;

    // search for a key in a vector of type/objects
    template <typename T, typename R = typename List<T>::const_iterator>
    R Find(List<T> &vector, T &key)
    {
        return std::find(vector.begin(), vector.end(), key);
    }

    template <typename T>
    bool Has(List<T> &vector, T &key)
    {
        return DarkEmperor::Find(vector, key) != vector.end();
    }

    template <typename T>
    bool In(List<T> &vector, T &key)
    {
        return DarkEmperor::Find(vector, key) != vector.end();
    }

    // unordered mapping
    template <typename T, typename R>
    using UnorderedMap = std::unordered_map<T, R>;

    // search for a key in a vector (of type/objects) and use a custom compare function
    template <typename T, typename R = typename List<T>::const_iterator>
    R Find(List<T> &vector, T &key, bool F(T &, T &))
    {
        return std::find_if(vector.begin(), vector.end(), [&](T &f)
                            { return F(key, f); });
    }
}

#endif