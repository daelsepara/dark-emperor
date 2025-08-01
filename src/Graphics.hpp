#ifndef __GRAPHICS_HPP__
#define __GRAPHICS_HPP__

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "HexMap.hpp"
#include "Primitives.hpp"

#ifdef SDL_HINT_RENDER_LINE_METHOD
#undef SDL_HINT_RENDER_LINE_METHOD
#endif

#define SDL_HINT_RENDER_LINE_METHOD "1"

namespace Hex::Graphics
{
    // horizontal scan lines toggle
    bool ScanLinesEnabled = true;

    // base class of the graphics system
    class Base
    {
    public:
        // window
        SDL_Window *Window = nullptr;

        // renderer
        SDL_Renderer *Renderer = nullptr;

        // screen dimension (width)
        int Width = 1280;

        // screen dimension (height)
        int Height = 800;

        Base() {}
    };

    // initialize graphics system
    void CreateWindow(Uint32 flags, const char *title, Base &graphics)
    {
        // the window and renderer we'll be rendering to
        graphics.Window = nullptr;

        graphics.Renderer = nullptr;

        if (SDL_Init(flags) < 0)
        {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_DisplayMode mode;

            SDL_GetCurrentDisplayMode(0, &mode);

            graphics.Width = mode.w;

            graphics.Height = mode.h;

            SDL_CreateWindowAndRenderer(graphics.Width, graphics.Height, (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), &graphics.Window, &graphics.Renderer);

            if (graphics.Renderer)
            {
                SDL_SetRenderDrawBlendMode(graphics.Renderer, SDL_BLENDMODE_BLEND);
            }

            if (!graphics.Window || !graphics.Renderer)
            {
                std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            }
            else if (graphics.Window)
            {
                SDL_SetWindowTitle(graphics.Window, title);
            }
        }
    }

    // initialize graphics subsystem
    void Initialize(Base &graphics, const char *title)
    {
        Graphics::CreateWindow(SDL_INIT_VIDEO | SDL_INIT_AUDIO, title, graphics);

        // raise window
        SDL_RaiseWindow(graphics.Window);

        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    }

    // initialize graphics system and set window/screen title
    Graphics::Base Initialize(const char *title)
    {
        auto graphics = Graphics::Base();

        Graphics::Initialize(graphics, title);

        return graphics;
    }

    // fill entire screen with specified color
    void FillWindow(Graphics::Base &graphics, Uint32 color)
    {
        if (graphics.Renderer)
        {
            SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));

            SDL_RenderClear(graphics.Renderer);
        }
    }


    // add scan lines to display
    void Scanlines(Base &graphics)
    {
        if (ScanLinesEnabled)
        {
            SDL_Rect scanline;

            scanline.w = graphics.Width;

            scanline.h = 1;

            scanline.x = 0;

            SDL_SetRenderDrawColor(graphics.Renderer, 0, 0, 0, 0x40);

            for (auto i = 0; i < graphics.Height; i += 2)
            {
                scanline.y = i + 1;

                SDL_RenderFillRect(graphics.Renderer, &scanline);
            }
        }
    }

    void RenderNow(Base &graphics)
    {
        if (graphics.Renderer)
        {
            Graphics::Scanlines(graphics);

            SDL_RenderPresent(graphics.Renderer);
        }
    }

    // define an rectangle and prepare color
    SDL_Rect CreateRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        SDL_Rect rect;

        rect.w = w;

        rect.h = h;

        rect.x = x;

        rect.y = y;

        if (graphics.Renderer)
        {
            SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));
        }

        return rect;
    }

    // draw rectangle outline on screen
    void DrawRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        if (graphics.Renderer)
        {
            auto rect = Graphics::CreateRect(graphics, w, h, x, y, color);

            SDL_RenderDrawRect(graphics.Renderer, &rect);
        }
    }

    void DrawHex(Base &graphics, Points hex, int offset_x, int offset_y, Uint32 color)
    {
        SDL_Point points[7];

        for (auto i = 0; i < 6; i++)
        {
            points[i].x = hex[i].X + offset_x;

            points[i].y = hex[i].Y + offset_y;
        }

        points[6].x = hex[0].X + offset_x;

        points[6].y = hex[0].Y + offset_y;

        SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));

        SDL_RenderDrawLines(graphics.Renderer, points, 7);
    }

    // draw line
    void DrawLine(Base &graphics, Point start, Point end, Point offset, Uint32 color)
    {
        SDL_Point points[2];

        points[0].x = start.X + offset.X;

        points[0].y = start.Y + offset.Y;

        points[1].x = end.X + offset.X;

        points[1].y = end.Y + offset.Y;

        SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));

        SDL_RenderDrawLines(graphics.Renderer, points, 2);
    }

    void DrawHex(Base &graphics, Points hex, Uint32 color)
    {
        Graphics::DrawHex(graphics, hex, 0, 0, color);
    }

    // return points comprising the line (x0, y0) - (x1, y1)
    Points Line(int x0, int y0, int x1, int y1)
    {
        auto points = Points();

        auto dx = std::abs(x1 - x0);

        auto sx = x0 < x1 ? 1 : -1;

        auto dy = std::abs(y1 - y0);

        auto sy = y0 < y1 ? 1 : -1;

        auto err = (dx > dy ? dx : -dy) / 2;

        auto e2 = 0;

        while (true)
        {
            points.push_back(Point(x0, y0));

            if (x0 == x1 && y0 == y1)
            {
                break;
            }

            e2 = err;

            if (e2 > -dx)
            {
                err -= dy;

                x0 += sx;
            }

            if (e2 < dy)
            {
                err += dx;

                y0 += sy;
            }
        }

        return points;
    }

    void PaintHex(Base &graphics, Points hex, Point offset, Uint32 color, bool flat)
    {
        if (flat)
        {
            auto tl = Graphics::Line(hex[4].X, hex[4].Y, hex[3].X, hex[3].Y);

            auto tr = Graphics::Line(hex[5].X, hex[5].Y, hex[0].X, hex[0].Y);

            for (auto i = 0; i < std::min(tl.size(), tr.size()); i++)
            {
                Graphics::DrawLine(graphics, tl[i], tr[i], offset, color);
            }

            auto bl = Graphics::Line(hex[3].X, hex[3].Y, hex[2].X, hex[2].Y);

            auto br = Graphics::Line(hex[0].X, hex[0].Y, hex[1].X, hex[1].Y);

            for (auto i = 0; i < std::min(bl.size(), br.size()); i++)
            {
                Graphics::DrawLine(graphics, bl[i], br[i], offset, color);
            }
        }
        else
        {
            auto tl = Graphics::Line(hex[3].X, hex[3].Y, hex[2].X, hex[2].Y);

            auto bl = Graphics::Line(hex[4].X, hex[4].Y, hex[5].X, hex[5].Y);

            for (auto i = 0; i < std::min(tl.size(), bl.size()); i++)
            {
                Graphics::DrawLine(graphics, tl[i], bl[i], offset, color);
            }

            auto tr = Graphics::Line(hex[2].X, hex[2].Y, hex[1].X, hex[1].Y);

            auto br = Graphics::Line(hex[5].X, hex[5].Y, hex[0].X, hex[0].Y);

            for (auto i = 0; i < std::min(tr.size(), br.size()); i++)
            {
                Graphics::DrawLine(graphics, tr[i], br[i], offset, color);
            }
        }
    }

    // base render texture function
    void Render(Base &graphics, SDL_Texture *texture, int texture_w, int texture_h, int x, int y, int bounds, int offset, int w, int h, Uint32 background)
    {
        if (graphics.Renderer)
        {
            SDL_Rect src;

            src.w = texture_w;

            src.h = std::min(texture_h, bounds);

            src.y = offset;

            src.x = 0;

            SDL_Rect dst;

            dst.w = w;

            dst.h = h;

            dst.x = x;

            dst.y = y;

            if (background != 0)
            {
                SDL_SetRenderDrawColor(graphics.Renderer, Color::R(background), Color::G(background), Color::B(background), Color::A(background));

                SDL_RenderFillRect(graphics.Renderer, &dst);
            }

            if (texture)
            {
                SDL_RenderCopy(graphics.Renderer, texture, &src, &dst);
            }
        }
    }

    // render texture at location
    void Render(Base &graphics, SDL_Texture *texture, int x, int y)
    {
        // , int texture_w, int texture_h, int x, int y, int bounds, int offset, int w, int h, Uint32 background)
        auto texture_w = 0;

        auto texture_h = 0;

        Hex::Size(texture, &texture_w, &texture_h);

        Graphics::Render(graphics, texture, texture_w, texture_h, x, y, texture_h, 0, texture_w, texture_h, 0);
    }

    // handle window events
    void HandleWindowEvent(SDL_Event &result, Graphics::Base &graphics)
    {
        if (result.window.event == SDL_WINDOWEVENT_RESTORED || result.window.event == SDL_WINDOWEVENT_MAXIMIZED || result.window.event == SDL_WINDOWEVENT_SHOWN)
        {
            SDL_RenderPresent(graphics.Renderer);
        }
    }

    // close graphics system
    void Quit(Base &graphics)
    {
        if (graphics.Renderer != nullptr)
        {
            SDL_DestroyRenderer(graphics.Renderer);

            graphics.Renderer = nullptr;
        }

        if (graphics.Window != nullptr)
        {
            SDL_DestroyWindow(graphics.Window);

            graphics.Window = nullptr;
        }

        IMG_Quit();

        if (SDL_WasInit(SDL_INIT_GAMECONTROLLER))
        {
            SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
        }

        if (SDL_WasInit(SDL_INIT_AUDIO))
        {
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }

        if (SDL_WasInit(SDL_INIT_VIDEO))
        {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        SDL_Quit();
    }
}

#endif
