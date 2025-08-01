#include "Graphics.hpp"
#include "HexMap.hpp"
#include "Asset.hpp"

namespace Hex
{
    void Main()
    {
        auto graphics = Graphics::Initialize("Hex Test");

        auto texture = Hex::Create(graphics.Renderer, "images/ninja-head.png");

        auto tw = Hex::Width(texture);

        auto th = Hex::Height(texture);

        auto map = Hex::Map(12, 8, 54, true);

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

                    Graphics::DrawHex(graphics, hex, map.Draw.X + cx, map.Draw.Y + cy, Color::Highlight);

                    auto show = false;

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
                        auto texture_x = cx - tw / 2;

                        auto texture_y = cy - th / 2;

                        Graphics::Render(graphics, texture, map.Draw.X + texture_x, map.Draw.Y + texture_y);

                        Graphics::DrawRect(graphics, 64, 64, map.Draw.X + texture_x, map.Draw.Y + texture_y, Color::Active);
                    }
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

        Hex::Free(&texture);

        Graphics::Quit(graphics);
    }
}

int main(int argc, char **argv)
{
    Hex::Main();

    return 0;
}
