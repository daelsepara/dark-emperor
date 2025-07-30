#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include "Primitives.hpp"

namespace Hex::Asset
{
    // create texture from a file
    SDL_Texture *Create(SDL_Renderer *renderer, const char *path)
    {
        SDL_Texture *texture = nullptr;

        auto surface = Hex::Load(path);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            if (texture)
            {
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                SDL_SetTextureColorMod(texture, Color::R(Color::Active), Color::G(Color::Active), Color::B(Color::Active));
            }

            Hex::Free(&surface);
        }

        return texture;
    }
}

#endif
