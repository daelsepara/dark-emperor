#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Map.hpp"
#include "Move.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

namespace DarkEmperor::Game
{
    enum class Feature
    {
        NONE = -1,
        NO_ATTRITION,
        UNLIMITED_STACK_SIZE,
        NO_STACKING
    };

    // check if other units are exerting control over this tile
    bool IsControlled(Map &map, Point point, Kingdom kingdom)
    {
        auto controlled = true;

        if (map.IsValid(point))
        {
            if (map[point].Others(kingdom) > 0)
            {
                controlled = false;
            }
            else
            {
                for (auto neighbor : map.Neighbors(point))
                {
                    auto &tile = map[point];

                    if (tile.Others(kingdom) > 0)
                    {
                        controlled = false;

                        break;
                    }
                }
            }
        }

        return controlled;
    }

    int Revenue(Map &map, Kingdom kingdom)
    {
        auto production = 0;

        auto lost = 0;

        for (auto y = 0; y < map.Dimensions.Y; y++)
        {
            for (auto x = 0; x < map.Dimensions.X; x++)
            {
                auto point = Point(x, y);

                auto &tile = map[point];

                if (tile.Owner == kingdom)
                {
                    if (!Game::IsControlled(map, point, kingdom))
                    {
                        // TODO: Tax according to terrain type
                        lost++;
                    }
                    else if (tile.IsCity() && tile.Count(kingdom) > 0)
                    {
                        production += tile.CityValue;
                    }
                }
            }
        }

        return std::max(0, production - lost);
    }

    // computes total attrition cost
    int Attrition(Map &map, Units &units, Points path)
    {
        auto attrition = 0;

        return attrition;
    }

    class Session
    {
    public:
        // list of game features
        List<Feature> Features = {};

        // units
        Units Units = {};

        // the world of Loslon
        Map Loslon = Map();

        // list of kingdoms that have been activated
        UnorderedMap<Kingdom, Player> Activations = {};

        Session()
        {
            // setup map dimensions
            this->Loslon = Map(40, 27, 54, true);

            // TODO: load tile settings
        }
    };
}

#endif
