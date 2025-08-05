#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <functional>
#include <SDL.h>

#include "Asset.hpp"
#include "Controls.hpp"

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

        Element() {}
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
    };

    typedef List<Reference<Scene>> Scenes;
}

#endif
