#include "Engine.hpp"
#include "Graphics.hpp"
#include "HexMap.hpp"
#include "Asset.hpp"

namespace DarkEmperor
{
    void Main(int width, int height, int size, bool flat, bool blind, bool new_textures, bool crop)
    {
        auto graphics = Graphics::Initialize("Hex Test");

        auto texture = DarkEmperor::Create(graphics.Renderer, "images/ninja-head.png");

        auto texture_w = DarkEmperor::Width(texture);

        auto texture_h = DarkEmperor::Height(texture);

        SDL_Texture *terrain = nullptr;

        if (new_textures)
        {
            terrain = DarkEmperor::Create(graphics.Renderer, flat ? "images/grass.png" : "images/grass_alt.png");
        }
        else if (crop)
        {
            terrain = DarkEmperor::Create(graphics.Renderer, "images/desert.png");
        }
        else
        {
            terrain = DarkEmperor::Create(graphics.Renderer, flat ? "images/terrain.png" : "images/terrain_alt.png");
        }

        auto terrain_w = DarkEmperor::Width(terrain);

        auto terrain_h = DarkEmperor::Height(terrain);

        // works best only with boundary rendering, i.e. rendering texture within hex boundaries
        if (crop)
        {
            // use only a portion of the texture
            if (flat)
            {
                terrain_w = size * 2;

                // pad
                terrain_h = size * DarkEmperor::Scale + 1;
            }
            else
            {
                // pad
                terrain_w = size * DarkEmperor::Scale + 1;

                terrain_h = size * 2;
            }
        }

        auto map = DarkEmperor::Map(width, height, size, flat);

        map.View = Point(0, 0);

        map.Limit = map.Dimensions - map.View * 2;

        if (map.Flat)
        {
            map.Draw.X = int(graphics.Width - (map.Limit.X * 3 + 1) * map.Size / 2) / 2 + map.Size;

            map.Draw.Y = int(graphics.Height - (map.Limit.Y * 2 + 1) * map.Size * DarkEmperor::Scale / 2) / 2;
        }
        else
        {
            map.Draw.X = int(graphics.Width - (map.Limit.X * 2 + 1) * map.Size * DarkEmperor::Scale / 2) / 2;

            map.Draw.Y = int(graphics.Height - (map.Limit.Y * 3 + 1) * map.Size / 2) / 2 + map.Size;
        }

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

        while (true)
        {
            Graphics::FillWindow(graphics, Color::Background);

            for (auto y = map.View.Y; y < map.View.Y + map.Limit.Y; y++)
            {
                auto cy = 0;

                for (auto x = map.View.X; x < map.View.X + map.Limit.X; x++)
                {
                    auto point = map[Point(x, y)].Point - map.View;

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

                    auto show = false;

                    // show hex "occupant"
                    if (map.Flat)
                    {
                        show = (point.Y % 3 == 0 && point.X % 2 == 0) || (point.Y % 3 == 1 && point.X % 2 == 1);
                    }
                    else
                    {
                        show = (point.X % 3 == 0 && point.Y % 2 == 0) || (point.X % 3 == 1 && point.Y % 2 == 1);
                    }

                    if (show)
                    {
                        // draw filled hex
                        Graphics::RenderHex(graphics, hex, map.Draw + Point(cx, cy), Color::Inactive, map.Flat);

                        auto texture_x = cx - texture_w / 2;

                        auto texture_y = cy - texture_h / 2;

                        auto offset = map.Draw + Point(texture_x, texture_y);

                        // show texture
                        Graphics::RenderTexture(graphics, texture, offset);

                        Graphics::DrawRect(graphics, texture_w, texture_h, offset, Color::Highlight);
                    }
                    else
                    {
                        // calculate terrain tile offsets
                        auto terrain_x = cx - terrain_w / 2;

                        auto terrain_y = cy - terrain_h / 2;

                        auto offset = map.Draw + Point(terrain_x, terrain_y);

                        if (blind)
                        {
                            // render texture at location
                            Graphics::RenderTexture(graphics, terrain, offset);
                        }
                        else
                        {
                            // render texture within hex boundaries
                            Graphics::RenderHex(graphics, terrain, offset_hex, offset, map.Flat);
                        }
                    }

                    // draw hex outline
                    Graphics::DrawPolygon(graphics, hex, map.Draw + Point(cx, cy), Color::Active);
                }
            }

            Graphics::RenderNow(graphics);

            SDL_Event result;

            SDL_WaitEventTimeout(&result, 25);

            if (result.type == SDL_QUIT)
            {
                break;
            }
            else if (result.type == SDL_WINDOWEVENT)
            {
                Graphics::HandleWindowEvent(result, graphics);
            }
            else if (result.type == SDL_KEYDOWN)
            {
                if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    break;
                }
            }
            else if (result.type == SDL_CONTROLLERBUTTONUP)
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    break;
                }
            }
            else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
            {
                break;
            }

            SDL_FlushEvent(result.type);
        }

        DarkEmperor::Free(&terrain);

        DarkEmperor::Free(&texture);

        Graphics::Quit(graphics);
    }
}

int main(int argc, char **argv)
{
    if (argc < 7)
    {
        std::cerr << "To Use:" << std::endl
                  << std::endl
                  << argv[0] << " [width] [height] [size in pixels] [flat/pointy orientation] [blind/boundary] [new/old/crop textures]" << std::endl;

        exit(1);
    }

    auto width = std::atoi(argv[1]);

    auto height = std::atoi(argv[2]);

    auto size = std::atoi(argv[3]);

    auto flat = DarkEmperor::Engine::ToUpper(argv[4]) == "FLAT";

    auto blind = DarkEmperor::Engine::ToUpper(argv[5]) == "BLIND";

    auto new_textures = DarkEmperor::Engine::ToUpper(argv[6]) == "NEW";

    auto crop = DarkEmperor::Engine::ToUpper(argv[6]) == "CROP";

    DarkEmperor::Main(width, height, size, flat, blind, new_textures, crop);

    return 0;
}
