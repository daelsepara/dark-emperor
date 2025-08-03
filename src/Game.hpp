#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Map.hpp"
#include "Move.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

namespace DarkEmperor::Game
{
    // check if other units are exerting control over this tile
    bool IsControlled(Map &map, Point point, Kingdom kingdom)
    {
        auto controlled = true;

        if (map.IsValid(point))
        {
            for (auto neighbor : map.Neighbors(point))
            {
                auto &tile = map[point];
            }
        }

        return controlled;
    }

    int Production(Map &map, Kingdom kingdom)
    {
        int production = 0;

        return production;
    }
}

#endif
