#include "Graphics.hpp"
#include "HexMap.hpp"
#include "Asset.hpp"

namespace HexMap
{
    void Main()
    {
        auto graphics = Graphics::Initialize("HexMap Test");

        auto size = 54;

        auto hex_scale = std::sqrt(3.0);

        auto texture = HexMap::Create(graphics.Renderer, "images/ninja-head.png");

        auto tw = HexMap::Width(texture);

        auto th = HexMap::Height(texture);

        auto size_x = 12;

        auto size_r = 8;

        auto offset_x = int(graphics.Width - ((size_x * 2 + 1) * size * hex_scale) / 2) / 2;

        auto offset_y = int(graphics.Height - ((size_r * 3 + 1) * size) / 2) / 2 + size;

        auto hex = HexMap::Tile(Point(0, 0), size);

        while (true)
        {
            Graphics::FillWindow(graphics, Color::Background);

            for (auto y = 0; y < size_r; y++)
            {
                auto hex_offset = hex_scale / 2.0 * (y % 2 + 1);

                auto cy = y * (3.0 / 2.0) * size;

                for (auto x = 0; x < size_x; x++)
                {
                    auto cx = (hex_scale * x + hex_offset) * size;

                    Graphics::DrawHex(graphics, hex, cx + offset_x, cy + offset_y, Color::Highlight);

                    if ((x % 3 == 0 && y % 2 == 0) || (x % 3 == 1 && y % 2 == 1))
                    {
                        auto texture_x = int(cx) - tw / 2;

                        auto texture_y = int(cy) - th / 2;

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

        HexMap::Free(&texture);

        Graphics::Quit(graphics);
    }
}

int main(int argc, char **argv)
{
    HexMap::Main();

    return 0;
}
