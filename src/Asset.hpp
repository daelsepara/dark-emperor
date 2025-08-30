#ifndef __ASSET_HPP__
#define __ASSET_HPP__

#include <iostream>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "nlohmann/json.hpp"

#include "Color.hpp"
#include "Templates.hpp"

namespace DarkEmperor::Asset
{
    const int NONE = -1;

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

        Asset::Size(texture, &width, nullptr);

        return width;
    }

    int Height(SDL_Texture *texture)
    {
        auto height = 0;

        Asset::Size(texture, nullptr, &height);

        return height;
    }

    // create texture from a file
    SDL_Texture *Create(SDL_Renderer *renderer, const char *path)
    {
        SDL_Texture *texture = nullptr;

        auto surface = Asset::Load(path);

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            if (texture)
            {
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                SDL_SetTextureColorMod(texture, Color::R(Color::Active), Color::G(Color::Active), Color::B(Color::Active));
            }

            Asset::Free(&surface);
        }

        return texture;
    }

    UnorderedMap<int, SDL_Texture *> Textures = {};

    UnorderedMap<std::string, int> Ids = {};

    template <typename T, typename R>
    void Free(UnorderedMap<T, R> &resources)
    {
        if (!resources.empty())
        {
            for (auto &resource : resources)
            {
                Asset::Free(&resource.second);
            }

            resources.clear();
        }
    }

    // retrieve texture based on numeric id
    SDL_Texture *Get(int asset)
    {
        return DarkEmperor::Has(Asset::Textures, asset) ? Asset::Textures[asset] : nullptr;
    }

    // retrieve texture numeric id based on string id
    int Id(std::string asset)
    {
        return DarkEmperor::Has(Asset::Ids, asset) ? Asset::Ids[asset] : NONE;
    }

    // retrieve texture based on string id
    SDL_Texture *Get(std::string asset)
    {
        return DarkEmperor::Has(Asset::Ids, asset) ? Asset::Get(Asset::Id(asset)) : nullptr;
    }

    // clears (frees) all textures
    void ClearTextures()
    {
        // LOG
        std::cerr << "Unloading " << Asset::Textures.size() << " texture(s) ..." << std::endl;

        Asset::Free(Asset::Textures);

        Asset::Ids.clear();
    }

    void LoadTextures(SDL_Renderer *renderer, const char *assets)
    {
        Asset::ClearTextures();

        std::ifstream ifs(assets);

        if (ifs.good())
        {
            // LOG
            std::cerr << "Loading textures from " << assets << " ..." << std::endl;

            auto data = nlohmann::json::parse(ifs);

            if (!data["assets"].is_null() && data["assets"].is_array() && data["assets"].size() > 0)
            {
                auto id = 0;

                for (auto i = 0; i < data["assets"].size(); i++)
                {
                    auto asset = !data["assets"][i]["id"].is_null() ? std::string(data["assets"][i]["id"]) : "";

                    auto path = !data["assets"][i]["path"].is_null() ? std::string(data["assets"][i]["path"]) : "";

                    if (!asset.empty() && !path.empty())
                    {
                        auto texture = Asset::Create(renderer, path.c_str());

                        if (texture != nullptr)
                        {
                            Asset::Ids[asset] = id;

                            Asset::Textures[id] = texture;

                            id++;
                        }
                    }
                }
            }

            ifs.close();

            // LOG
            std::cerr << "Loaded " << Asset::Textures.size() << " image(s) ..." << std::endl;
        }
    }

    void LoadTextures(SDL_Renderer *renderer, std::string assets)
    {
        Asset::LoadTextures(renderer, assets.c_str());
    }
}

#endif
