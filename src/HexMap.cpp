#include "Engine.hpp"
#include "Graphics.hpp"
#include "HexMap.hpp"
#include "Asset.hpp"

namespace Hex
{
    void Main(int width, int height, int size, bool flat, bool blind, bool new_textures, bool crop)
    {
        auto graphics = Graphics::Initialize("Hex Test");

        auto texture = Hex::Create(graphics.Renderer, "images/ninja-head.png");

        auto texture_w = Hex::Width(texture);

        auto texture_h = Hex::Height(texture);

        SDL_Texture *terrain = nullptr;

        if (new_textures)
        {
            terrain = Hex::Create(graphics.Renderer, flat ? "images/grass.png" : "images/grass_alt.png");
        }
        else if (crop)
        {
            terrain = Hex::Create(graphics.Renderer, "images/desert.png");
        }
        else
        {
            terrain = Hex::Create(graphics.Renderer, flat ? "images/terrain.png" : "images/terrain_alt.png");
        }

        auto terrain_w = Hex::Width(terrain);

        auto terrain_h = Hex::Height(terrain);

        // works best only with boundary rendering, i.e. rendering texture within hex boundaries
        if (crop)
        {
            // use only a portion of the texture
            if (flat)
            {
                terrain_w = size * 2;

                // pad
                terrain_h = size * Hex::Scale + 1;
            }
            else
            {
                // pad
                terrain_w = size * Hex::Scale + 1;

                terrain_h = size * 2;
            }
        }

        auto map = Hex::Map(width, height, size, flat);

        map.View = Point(0, 0);

        map.Limit = map.Dimensions - map.View * 2;

        if (map.Flat)
        {
            map.Draw.X = int(graphics.Width - (map.Limit.X * 3 + 1) * map.Size / 2) / 2 + map.Size;

            map.Draw.Y = int(graphics.Height - (map.Limit.Y * 2 + 1) * map.Size * Hex::Scale / 2) / 2;
        }
        else
        {
            map.Draw.X = int(graphics.Width - (map.Limit.X * 2 + 1) * map.Size * Hex::Scale / 2) / 2;

            map.Draw.Y = int(graphics.Height - (map.Limit.Y * 3 + 1) * map.Size / 2) / 2 + map.Size;
        }

        auto hex = Hex::Vertices(Point(0, 0), map.Size, map.Flat);

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
                        auto hex_offset = Hex::Scale / 2.0 * (point.X % 2 + 1);

                        cy = int((Hex::Scale * point.Y + hex_offset) * map.Size);

                        cx = int(point.X * Hex::Offset * map.Size);
                    }
                    else
                    {
                        auto hex_offset = Hex::Scale / 2.0 * (point.Y % 2 + 1);

                        cx = int((Hex::Scale * point.X + hex_offset) * map.Size);

                        cy = int(point.Y * Hex::Offset * map.Size);
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
                        Graphics::PaintHex(graphics, hex, map.Draw + Point(cx, cy), Color::Inactive, map.Flat);

                        auto texture_x = cx - texture_w / 2;

                        auto texture_y = cy - texture_h / 2;

                        // show texture
                        Graphics::RenderTexture(graphics, texture, map.Draw.X + texture_x, map.Draw.Y + texture_y);

                        Graphics::DrawRect(graphics, 64, 64, map.Draw.X + texture_x, map.Draw.Y + texture_y, Color::Highlight);
                    }
                    else
                    {
                        // calculate terrain tile offsets
                        auto terrain_x = cx - terrain_w / 2;

                        auto terrain_y = cy - terrain_h / 2;

                        if (blind)
                        {
                            // render texture at location
                            Graphics::RenderTexture(graphics, terrain, map.Draw + Point(terrain_x, terrain_y));
                        }
                        else
                        {
                            // render texture within hex boundaries
                            Graphics::RenderHex(graphics, terrain, offset_hex, map.Draw + Point(terrain_x, terrain_y), map.Flat);
                        }
                    }

                    // draw hex outline
                    Graphics::DrawHex(graphics, hex, map.Draw.X + cx, map.Draw.Y + cy, Color::Active);
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

        Hex::Free(&terrain);

        Hex::Free(&texture);

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

    auto flat = Hex::Engine::ToUpper(argv[4]) == "FLAT";

    auto blind = Hex::Engine::ToUpper(argv[5]) == "BLIND";

    auto new_textures = Hex::Engine::ToUpper(argv[6]) == "NEW";

    auto crop = Hex::Engine::ToUpper(argv[6]) == "CROP";

    Hex::Main(width, height, size, flat, blind, new_textures, crop);

    return 0;
}
