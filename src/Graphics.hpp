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

#define SDL_HINT_RENDER_LINE_METHOD "2"

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

    // send to hardware / software renderer
    void RenderNow(Base &graphics)
    {
        if (graphics.Renderer)
        {
            // show scanlines (if enabled)
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

    // draw rectangle outline
    void DrawRect(Graphics::Base &graphics, int w, int h, int x, int y, int color)
    {
        if (graphics.Renderer)
        {
            auto rect = Graphics::CreateRect(graphics, w, h, x, y, color);

            SDL_RenderDrawRect(graphics.Renderer, &rect);
        }
    }

    // draw rectangle outline
    void DrawRect(Graphics::Base &graphics, int w, int h, Point point, int color)
    {
        Graphics::DrawRect(graphics, w, h, point.X, point.Y, color);
    }

    // draw line
    void DrawLine(Base &graphics, Point start, Point end, Point offset, Uint32 color)
    {
        SDL_Point points[2];

        // translate points using offset and convert to SDL's point formats
        points[0].x = start.X + offset.X;

        points[0].y = start.Y + offset.Y;

        points[1].x = end.X + offset.X;

        points[1].y = end.Y + offset.Y;

        SDL_SetRenderDrawColor(graphics.Renderer, Color::R(color), Color::G(color), Color::B(color), Color::A(color));

        SDL_RenderDrawLines(graphics.Renderer, points, 2);
    }

    // draw polygon outline
    void DrawPolygon(Base &graphics, Points polygon, int offset_x, int offset_y, Uint32 color)
    {
        for (auto vertex = 0; vertex < polygon.size() - 1; vertex++)
        {
            Graphics::DrawLine(graphics, polygon[vertex], polygon[vertex + 1], Point(offset_x, offset_y), color);
        }

        Graphics::DrawLine(graphics, polygon[polygon.size() - 1], polygon[0], Point(offset_x, offset_y), color);
    }

    // draw polygon outline
    void DrawPolygon(Base &graphics, Points polygon, Point offset, Uint32 color)
    {
        Graphics::DrawPolygon(graphics, polygon, offset.X, offset.Y, color);
    }

    // draw polygon outline
    void DrawPolygon(Base &graphics, Points polygon, Uint32 color)
    {
        Graphics::DrawPolygon(graphics, polygon, 0, 0, color);
    }

    // return points comprising the line (x0, y0) - (x1, y1) using Bresenham's line algorithm
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

    // return points comprising the line (start) - (end) using Bresenham's line algorithm
    Points Line(Point start, Point end)
    {
        return Graphics::Line(start.X, start.Y, end.X, end.Y);
    }

    // render one line of texture within the boundary (start, end)
    void RenderLineTexture(Base &graphics, SDL_Texture *texture, Point start, Point end, Point offset)
    {
        SDL_Rect src, dst;

        // render one line from texture (horizontal or vertical)
        src.w = (end.X == start.X) ? 1 : (end.X - start.X);

        src.h = (end.Y == start.Y) ? 1 : (end.Y - start.Y);

        src.x = start.X;

        src.y = start.Y;

        dst.w = src.w;

        dst.h = src.h;

        dst.x = offset.X + start.X;

        dst.y = offset.Y + start.Y;

        // copy one line of the texture (src) into a new location (dst)
        SDL_RenderCopy(graphics.Renderer, texture, &src, &dst);
    }

    // render hex line by line within boundary (start, end)
    void RenderHex(Base &graphics, SDL_Texture *texture, Points hex, Point offset, Uint32 color, bool flat)
    {
        // determine rendering method: flat (top to bottom) or pointy (left to right)

        // side1, side2 => flat (top to midddle), pointy (left to middle)
        auto side1 = flat ? Graphics::Line(hex[4], hex[3]) : Graphics::Line(hex[4], hex[5]);

        auto side2 = flat ? Graphics::Line(hex[5], hex[0]) : Graphics::Line(hex[3], hex[2]);

        // side3, side4 => flat (middle to bottom), pointy (middle to right)
        auto side3 = flat ? Graphics::Line(hex[3], hex[2]) : Graphics::Line(hex[5], hex[0]);

        auto side4 = flat ? Graphics::Line(hex[0], hex[1]) : Graphics::Line(hex[2], hex[1]);

        // render texture / lines on first half of the hex
        for (auto i = 0; i < std::min(side1.size(), side2.size()); i++)
        {
            if (texture)
            {
                Graphics::RenderLineTexture(graphics, texture, side1[i], side2[i], offset);
            }
            else
            {
                Graphics::DrawLine(graphics, side1[i], side2[i], offset, color);
            }
        }

        // render texture / lines on second half of the hex
        for (auto i = 0; i < std::min(side3.size(), side4.size()); i++)
        {
            if (texture)
            {
                Graphics::RenderLineTexture(graphics, texture, side3[i], side4[i], offset);
            }
            else
            {
                Graphics::DrawLine(graphics, side3[i], side4[i], offset, color);
            }
        }
    }

    // draw a filled hex
    void RenderHex(Base &graphics, Points hex, Point offset, Uint32 color, bool flat)
    {
        Graphics::RenderHex(graphics, nullptr, hex, offset, color, flat);
    }

    // render texture within hex boundaries
    void RenderHex(Base &graphics, SDL_Texture *texture, Points hex, Point offset, bool flat)
    {
        Graphics::RenderHex(graphics, texture, hex, offset, 0, flat);
    }

    // base render texture function
    void RenderTexture(Base &graphics, SDL_Texture *texture, int texture_w, int texture_h, int x, int y, int bounds, int offset, int w, int h, Uint32 background)
    {
        if (graphics.Renderer)
        {
            SDL_Rect src, dst;

            src.w = texture_w;

            src.h = std::min(texture_h, bounds);

            src.y = offset;

            src.x = 0;

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
    void RenderTexture(Base &graphics, SDL_Texture *texture, int w, int h, int x, int y)
    {
        Graphics::RenderTexture(graphics, texture, w, h, x, y, h, 0, w, h, 0);
    }

    // render texture at location
    void RenderTexture(Base &graphics, SDL_Texture *texture, int x, int y)
    {
        auto texture_w = 0;

        auto texture_h = 0;

        Hex::Size(texture, &texture_w, &texture_h);

        Graphics::RenderTexture(graphics, texture, texture_w, texture_h, x, y);
    }

    // render texture at location
    void RenderTexture(Base &graphics, SDL_Texture *texture, Point point)
    {
        Graphics::RenderTexture(graphics, texture, point.X, point.Y);
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
