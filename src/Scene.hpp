#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <functional>
#include <SDL.h>

#include "Asset.hpp"
#include "Controls.hpp"
#include "Map.hpp"

// classes and functions to define objects that will be rendered on screen
namespace DarkEmperor
{
    // scene element base class
    class Element
    {
    public:
        // location on screen
        Point Location = Point(0, 0);

        // dimensions
        Point Dimensions = Point(0, 0);

        // texture associated with the element
        SDL_Texture *Texture;

        // background color, 0 if none
        Uint32 Background = 0;

        // border color, 0 if none
        Uint32 Border = 0;

        // size of texture portion to be rendered. Used with Offset.
        int Bounds = 0;

        // starting point in texture to be rendered. Used with Bounds.
        int Offset = 0;

        // thickness of the border in pixels
        int BorderSize = 0;

        // hex
        Points Hex = {};

        // flat / pointed orientation
        bool Flat = false;

        Element() {}

        Element(SDL_Texture *texture, Point location) : Location(location), Texture(texture)
        {
            Asset::Size(this->Texture, &this->Dimensions.X, &this->Dimensions.Y);
        }
    };

    typedef List<Element> Elements;

    typedef List<Controls::Base> ControlsList;

    class Scene
    {
    public:
        Elements Elements = {};

        ControlsList Controls = {};

        // background color of the entire screen. set before rendering each element
        Uint32 Background = 0;

        // clipping area
        Point Clip = Point(-1, -1);

        Point ClipDimensions = Point(0, 0);

        Scene() {}

        void Add(Element element) { this->Elements.push_back(element); }

        void Add(Controls::Base control) { this->Controls.push_back(control); }
    };

    typedef List<Reference<Scene>> Scenes;

    Scene MapScene(Map &map, Units &units, Uint32 background = 0)
    {
        auto scene = Scene();

        scene.Background = background;

        // precalculate hex (on-screen) coordinates and offset
        auto hex = DarkEmperor::Vertices(Point(0, 0), map.Size, map.Flat);

        auto offset_hex = Points();

        auto min_x = hex[0].X;

        auto min_y = hex[0].Y;

        for (auto i = 0; i < hex.size(); i++)
        {
            min_x = std::min(min_x, hex[i].X);

            min_y = std::min(min_y, hex[i].Y);
        }

        for (auto i = 0; i < hex.size(); i++)
        {
            offset_hex.push_back(hex[i] - Point(min_x, min_y));
        }

        for (auto y = map.View.Y; y < map.View.Y + map.Limit.Y; y++)
        {
            auto cy = 0;

            for (auto x = map.View.X; x < map.View.X + map.Limit.X; x++)
            {
                auto &tile = map[Point(x, y)];

                auto point = tile.Point - map.View;

                auto cx = 0;

                // calculate hex center locations
                if (map.Flat)
                {
                    auto hex_offset = DarkEmperor::Scale / 2.0 * (point.X % 2 + 1);

                    cy = int((DarkEmperor::Scale * point.Y + hex_offset) * map.Size);

                    cx = int(point.X * DarkEmperor::Offset * map.Size);
                }
                else
                {
                    auto hex_offset = DarkEmperor::Scale / 2.0 * (point.Y % 2 + 1);

                    cx = int((DarkEmperor::Scale * point.X + hex_offset) * map.Size);

                    cy = int(point.Y * DarkEmperor::Offset * map.Size);
                }

                auto element = Element();

                element.Texture = Asset::Get(tile.Asset);

                if (element.Texture)
                {
                    // get texture dimensions
                    auto terrain_w = Asset::Width(element.Texture);

                    auto terrain_h = Asset::Height(element.Texture);

                    // calculate terrain tile offsets (to center it within the hex)
                    auto terrain_x = cx - terrain_w / 2;

                    auto terrain_y = cy - terrain_h / 2;

                    auto offset = map.Draw + Point(terrain_x, terrain_y);

                    element.Hex = DarkEmperor::Add(offset_hex, offset);
                }
                else
                {
                    // set hex colors
                    element.Background = tile.Background;

                    element.Border = tile.Border;

                    // add hex outline / background
                    element.Hex = DarkEmperor::Add(hex, map.Draw + Point(cx, cy));
                }

                // add hex
                scene.Add(element);

                // add unit/stack icon to the scene
                if (tile.Units.size() > 0)
                {
                    auto stack = Element();

                    auto first_asset = -1;

                    auto assets = 0;

                    for (auto &tile_unit : tile.Units)
                    {
                        if (tile_unit.Id >= 0 && tile_unit.Id < units.size())
                        {
                            auto &unit = units[tile_unit.Id];

                            if (unit.Type != UnitType::NONE && unit.Asset != Asset::NONE)
                            {
                                if (first_asset == Asset::NONE)
                                {
                                    first_asset = unit.Asset;
                                }

                                assets++;
                            }
                        }
                    }

                    if (assets > 1)
                    {
                        stack.Texture = Asset::Get("MULTIPLE UNITS");
                    }
                    else
                    {
                        stack.Texture = Asset::Get(first_asset);

                        // TODO: add kingdom colors
                    }

                    if (stack.Texture)
                    {
                        // get texture dimensions
                        auto texture_w = Asset::Width(stack.Texture);

                        auto texture_h = Asset::Height(stack.Texture);

                        auto texture_x = cx - texture_w / 2;

                        auto texture_y = cy - texture_h / 2;

                        auto offset = map.Draw + Point(texture_x, texture_y);

                        stack.Location = offset;

                        stack.Dimensions = Point(texture_w, texture_h);
                    }

                    scene.Add(stack);
                }

                // add outline (on textured hex)
                if (element.Texture && tile.Border != 0)
                {
                    auto outline = Element();

                    outline.Border = tile.Border;

                    outline.Hex = DarkEmperor::Add(hex, map.Draw + Point(cx, cy));

                    scene.Add(outline);
                }
            }
        }

        return scene;
    }
}

#endif
