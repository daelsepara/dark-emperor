#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "Color.hpp"

namespace HexMap
{
    // load an image as an SDL surface
    SDL_Surface *Load(const char *image)
    {
        auto surface = IMG_Load(image);

        if (!surface)
        {
            std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
        }

        return surface;
    }

    // free surface
    void Free(SDL_Surface **surface)
    {
        if (*surface != nullptr)
        {
            SDL_FreeSurface(*surface);

            *surface = nullptr;
        }
    }

    // free texture
    void Free(SDL_Texture **texture)
    {
        if (*texture != nullptr)
        {
            SDL_DestroyTexture(*texture);

            *texture = nullptr;
        }
    }

    // create texture from a file
    SDL_Texture *Create(SDL_Renderer *renderer, const char *path)
    {
        SDL_Texture *texture = nullptr;

        auto surface = HexMap::Load(path);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            if (texture)
            {
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                SDL_SetTextureColorMod(texture, Color::R(Color::Active), Color::G(Color::Active), Color::B(Color::Active));
            }

            HexMap::Free(&surface);
        }

        return texture;
    }

    void Size(SDL_Texture *texture, int *texture_w, int *texture_h)
    {
        if (texture)
        {
            SDL_QueryTexture(texture, nullptr, nullptr, texture_w, texture_h);
        }
    }

    int Width(SDL_Texture *texture)
    {
        auto width = 0;

        HexMap::Size(texture, &width, nullptr);

        return width;
    }

    int Height(SDL_Texture *texture)
    {
        auto height = 0;

        HexMap::Size(texture, nullptr, &height);

        return height;
    }
}

#endif
