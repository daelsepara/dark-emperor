#include "Graphics.hpp"
#include "HexMap.hpp"
#include "Asset.hpp"

namespace Hex
{
    void Main()
    {
        auto graphics = Graphics::Initialize("Hex Test");

        auto size = 54;

        auto hex_scale = std::sqrt(3.0);

        auto texture = Hex::Create(graphics.Renderer, "images/ninja-head.png");

        auto tw = Hex::Width(texture);

        auto th = Hex::Height(texture);

        auto size_x = 12;

        auto size_y = 8;

        auto flat = false;

        auto offset_x = 0;

        auto offset_y = 0;

        if (flat)
        {
            offset_x = int(graphics.Width - ((size_x * 3 + 1) * size) / 2) / 2 + size;

            offset_y = int(graphics.Height - ((size_y * 2 + 1) * size * hex_scale) / 2) / 2;
        }
        else
        {
            offset_x = int(graphics.Width - ((size_x * 2 + 1) * size * hex_scale) / 2) / 2;

            offset_y = int(graphics.Height - ((size_y * 3 + 1) * size) / 2) / 2 + size;
        }

        auto hex = Hex::Vertices(Point(0, 0), size, flat);

        while (true)
        {
            Graphics::FillWindow(graphics, Color::Background);

            for (auto y = 0; y < size_y; y++)
            {
                auto cy = 0;

                for (auto x = 0; x < size_x; x++)
                {
                    auto cx = 0;

                    if (flat)
                    {
                        auto hex_offset = hex_scale / 2.0 * (x % 2 + 1);

                        cy = (hex_scale * y + hex_offset) * size;

                        cx = x * (3.0 / 2.0) * size;
                    }
                    else
                    {
                        auto hex_offset = hex_scale / 2.0 * (y % 2 + 1);

                        cx = (hex_scale * x + hex_offset) * size;

                        cy = y * (3.0 / 2.0) * size;
                    }

                    Graphics::DrawHex(graphics, hex, cx + offset_x, cy + offset_y, Color::Highlight);

                    auto show = false;

                    if (flat)
                    {
                        show = (y % 3 == 0 && x % 2 == 0) || (y % 3 == 1 && x % 2 == 1);
                    }
                    else
                    {
                        show = (x % 3 == 0 && y % 2 == 0) || (x % 3 == 1 && y % 2 == 1);
                    }

                    if (show)
                    {
                        auto texture_x = cx - tw / 2;

                        auto texture_y = cy - th / 2;

                        Graphics::Render(graphics, texture, texture_x + offset_x, texture_y + offset_y);

                        Graphics::DrawRect(graphics, 64, 64, texture_x + offset_x, texture_y + offset_y, Color::Active);
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
