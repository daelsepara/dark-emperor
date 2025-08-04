#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <SDL.h>

#include "Asset.hpp"
#include "Types.hpp"

// classes and functions to define objects that will be rendered on screen
namespace DarkEmperor
{
    // scene element base class
    class Element
    {
    public:
        // texture associated with the element
        SDL_Texture *Texture;

        // background color, 0 if none
        Uint32 Background = 0;

        // border color, 0 if none
        Uint32 Border = 0;

        // x location on screen
        int X = 0;

        // y location on screen
        int Y = 0;

        // size of texture portion to be rendered. Used with Offset.
        int Bounds = 0;

        // starting point in texture to be rendered. Used with Bounds.
        int Offset = 0;

        // width of the element
        int W = 0;

        // height of the element
        int H = 0;

        // thickness of the border in pixels
        int BorderSize = 0;

        // hex
        Points Hex = {};

        Element() {}
    };

    typedef List<Element> Elements;

    class Scene
    {
    public:
        Elements Elements = {};

        Scene() {}
    };
}

#endif
